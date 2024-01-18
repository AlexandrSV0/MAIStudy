#include <iostream>
#include <fstream>
#include <cmath>
#include <string>

#define CSC(call)                                                                                                                                       \
do {                                                                                                                                                            \
    cudaError_t status = call;                                                                                                                       \
    if (status != cudaSuccess) {                                                                                                                    \
        fprintf(stderr, "ERROR is %s:%d. Message: %s\n", __FILE__, __LINE__, cudaGetErrorString(status));  \
        exit(0);                                                                                                                                                \
    }                                                                                                                                                             \
} while(0)

const int POLYGONS_CNT = 26;
const int RGB_CONST = 255;
const int BUFF_SIZE = 256;
const double EPSILON = 1e-10;

const std::string DEFAULT_INPUT = R"""(126
output/img_%d.data
720 720 100
7.0 3.0 0.0		2.0 1.0     2.0 6.0 1.0     0.0 0.0
2.0 0.0 0.0     0.5 0.1     1.0 4.0 1.0     0.0 0.0
0 -3 0.3	0 0 1	    1.7
1 0.5 1		0.3 0.7		0.8     1
-1 3 0.5    1 0 0      1.5
-5 -5 -1    -5.0 5.0 -1.0    5.0 5.0 -1.0    5.0 -5.0 -1.0  0.0 1.0 0.0
-10.0 0.0 20.0     0.3 0.2 0.1
4
)""";


struct Figure {
    double3 center;
    double3 color;
    double radius;

    __host__ __device__ Figure() {}
    
    friend std::istream& operator>>(std::istream& is, Figure& f) {
        is >> f.center.x >> f.center.y >> f.center.z;
        is >> f.color.x >> f.color.y >> f.color.z;
        is >> f.radius;
        return is;
    }
};

struct Motion_params {
    double r0, z0, phi0, A, Az, wr, wz, wphi, pr, pz;
    __host__ __device__ Motion_params() {}
    
    friend std::istream& operator>>(std::istream& is, Motion_params& params) {
        is >> params.r0 >> params.z0 >> params.phi0 >> params.A >> params.Az >> params.wr >> params.wz >> params.wphi >> params.pr >> params.pz;
        return is;
    }

};

__host__ __device__ double dot(double3 a, double3 b) {    
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

__host__ __device__ double3 diff(double3 a, double3 b) {
	return {a.x - b.x, a.y - b.y, a.z - b.z};
}

__host__ __device__ double3 sum(double3 a, double3 b) {
	return {a.x + b.x, a.y + b.y, a.z + b.z};
}

__host__ __device__ uint4 sum(uint4 data, uchar4 cur_pixel) {
    data.x += cur_pixel.x;
    data.y += cur_pixel.y;
    data.z += cur_pixel.z;
    return data;
}

__host__ __device__ uint4 divide(uint4 data, int num) {
    data.x /= num;
    data.y /= num;
    data.z /= num;
    return data;
}

__host__ __device__ double3 prod(double3 a, double3 b) {   
    return make_double3(
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x
    );
}

__host__ __device__ double3 normalize(double3 v) {
    double l = sqrt(dot(v, v));
    return make_double3(v.x / l, v.y / l, v.z / l);
}

__host__ __device__ double3 mult(double3 a, double3 b, double3 c, double3 v) {
    return make_double3(
        a.x*v.x + b.x*v.y + c.x*v.z,
        a.y*v.x + b.y*v.y + c.y*v.z,
        a.z*v.x + b.z*v.y + c.z*v.z
    );
}

__host__ __device__ double3 mult(double3 a, double num) {
    return make_double3(a.x*num, a.y*num, a.z*num);
}


struct Polygon {
    double3 a;
    double3 b;
    double3 c;
    uchar4 color;

    __host__ __device__ Polygon() {}
    __host__ __device__ Polygon(double3 a, double3 b, double3 c, uchar4 color) : a(a), b(b), c(c), color(color) {}
};

__host__ __device__ uchar4 ray(Polygon *polygons, double3 pos, double3 dir, double3 lpos, uchar4 lcol) {
    const auto empty_uchar4 = make_uchar4(0, 0, 0, RGB_CONST);
    int k = -1, k_min = -1;
    double ts_min;
    while (++k < POLYGONS_CNT) {
        double3 e1 = diff(polygons[k].b, polygons[k].a);
        double3 e2 = diff(polygons[k].c, polygons[k].a);
        double3 p = prod(dir, e2);
        double div = dot(p, e1);

        if (fabs(div) < EPSILON) {
            continue;
        }

        double3 t = diff(pos, polygons[k].a);
        double u = dot(p, t) / div;
        if (u < 0.0 || u > 1.0) {
            continue;
        }

        double3 q = prod(t, e1);
        double v = dot(q, dir) / div;
        if (v < 0.0 || v + u > 1.0) {
            continue;
        }

        double ts = dot(q, e2) / div; 
        if (ts < 0.0) {
            continue;
        }
        
        if (k_min == -1 || ts < ts_min) {
            k_min = k;
            ts_min = ts;
        }
    }

    if (k_min == -1) {
        return empty_uchar4;
    }

    pos = sum(mult(dir, ts_min), pos);
    dir = diff(lpos, pos);
    double length = sqrt(dot(dir, dir));
    dir = normalize(dir);
    k = -1;
    while (++k < POLYGONS_CNT) { // we have to process the same way the light source
        double3 e1 = diff(polygons[k].b, polygons[k].a);
        double3 e2 = diff(polygons[k].c, polygons[k].a);
        double3 p = prod(dir, e2);
        double div = dot(p, e1);

        if (fabs(div) < EPSILON) {
            continue;
        }
        
        double3 t = diff(pos, polygons[k].a);
        double u = dot(p, t) / div;
        if (u < 0.0 || u > 1.0) {
            continue;
        }

        double3 q = prod(t, e1);
        double v = dot(q, dir) / div;
        if (v < 0.0 || v + u > 1.0) {
            continue;
        }

        double ts = dot(q, e2) / div; 
        if (ts > 0.0 && ts < length && k != k_min) {
            return empty_uchar4;
        }
    }

    uchar4 k_color = polygons[k_min].color;
    double x = k_color.x * lcol.x;
    double y = k_color.y * lcol.y;
    double z = k_color.z * lcol.z;
    return make_uchar4(x, y, z, RGB_CONST);
}

// CPU version
__host__ __device__ void make_cpu_render(uchar4 *data, Polygon *polygons, double3 cam_pos, double3 cam_view, int w, int h, double angle, double3 lpos, uchar4 lcol) {
    double dw = 2.0 / (w - 1.0);
    double dh = 2.0 / (h - 1.0);
    double z = 1.0 / tan(angle * M_PI / 360.0);

    double3 bz = normalize(diff(cam_view, cam_pos));
    double3 bx = normalize(prod(bz, {0.0, 0.0, 1.0}));
    double3 by = normalize(prod(bx, bz));

    int i = -1, j = -1;
    while (++i < w) {
        while (++j < h) {
            double3 v = make_double3(-1.0 + dw*i, (-1.0 + dh*j)*h / w, z);
            double3 dir = mult(bx, by, bz, v);
            data[(h - 1 - j) * w + i] = ray(polygons, cam_pos, normalize(dir), lpos, lcol);
        }
        j = -1;
    }
}

// GPU version
__global__ void kernel_render(uchar4 *data, Polygon *polygons, double3 cam_pos, double3 cam_view, int w, int h, double angle, double3 lpos, uchar4 lcol) {
    int id_x = blockDim.x * blockIdx.x + threadIdx.x;
    int id_y = blockDim.y * blockIdx.y + threadIdx.y;
    int offset_x = blockDim.x * gridDim.x;
    int offset_y = blockDim.y * gridDim.y;

    double dw = 2.0 / (w - 1.0);
    double dh = 2.0 / (h - 1.0);
    double z = 1.0 / tan(angle * M_PI / 360.0);

    double3 bz = normalize(diff(cam_view, cam_pos));
    double3 bx = normalize(prod(bz, {0.0, 0.0, 1.0}));
    double3 by = normalize(prod(bx, bz));

    for (int i = id_x; i < w; i += offset_x) {
        for (int j = id_y; j < h; j += offset_y) {
            double3 v = make_double3(-1.0 + dw * i, (-1.0 + dh * j) * h / w, z);
            double3 dir = mult(bx, by, bz, v);
            data[(h - 1 - j) * w + i] = ray(polygons, cam_pos, normalize(dir), lpos, lcol);
        }
    }
}

__host__ __device__ uint4 sum_pixels(uchar4 *data, int w, int h, int x, int y, int sqrtSamples) {
    auto block_data = make_uint4(0, 0, 0, 0);
    for (int i = 0; i < sqrtSamples; ++i)
        for (int j = 0; j < sqrtSamples; ++j)
            block_data = sum(block_data, data[w*sqrtSamples * (y*sqrtSamples + j) + (x*sqrtSamples + i)]);
    return block_data;
}

// CPU version
__host__ __device__ void make_cpu_ssaa(uchar4 *data, uchar4 *data_out, int w, int h, int sqrtSamples) {
    int pixelBlock = sqrtSamples*sqrtSamples;
    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            auto block_data = sum_pixels(data, w, h, x, y, sqrtSamples);
            block_data = divide(block_data, pixelBlock);
            data_out[y*w + x] = make_uchar4(block_data.x, block_data.y, block_data.z, RGB_CONST);
        }
    }
}

// GPU version
__global__ void kernel_ssaa(uchar4 *data, uchar4 *data_out, int w, int h, int sqrtSamples) {
    int id_x = blockDim.x * blockIdx.x + threadIdx.x;
    int id_y = blockDim.y * blockIdx.y + threadIdx.y;
    int offset_x = blockDim.x * gridDim.x;
    int offset_y = blockDim.y * gridDim.y;

    int pixelBlock = sqrtSamples * sqrtSamples;
    for (int x = id_x; x < w; x += offset_x) {
        for (int y = id_y; y < h; y += offset_y) { 
            auto block_data = sum_pixels(data, w, h, x, y, sqrtSamples);
            block_data = divide(block_data, pixelBlock);
            data_out[y * w + x] = make_uchar4(block_data.x, block_data.y, block_data.z, RGB_CONST);
        }
    }
}

// building scene and figure objects
void build_space(Polygon *polygons, double3 *floor, uchar4 color, int &start) {
    polygons[start] = Polygon(floor[0], floor[1], floor[2], color);
    polygons[++start] = Polygon(floor[0], floor[2], floor[3], color);
}

void build_tetrahedron(Polygon *polygons, double3 center, uchar4 color, double r, int &start) {
    double a = r * sqrt(3);
    double half_a = a / 2;
    double quarter_a = a / 4;

    auto p1 = make_double3(center.x - half_a, center.y, center.z - quarter_a);
    auto p2 = make_double3(center.x, center.y + r, center.z - quarter_a);
    auto p3 = make_double3(center.x + half_a, center.y, center.z - quarter_a);
    auto p4 = make_double3(center.x, center.y, center.z + r);

    polygons[++start] = Polygon(p1, p2, p3, color);
    polygons[++start] = Polygon(p1, p2, p4, color);
    polygons[++start] = Polygon(p1, p3, p4, color);
    polygons[++start] = Polygon(p2, p3, p4, color);
}

void build_hexahedron(Polygon *polygons, double3 center, uchar4 color, double r, int &start) {
    double a = 3 * r / sqrt(3);
    
    auto p0 = make_double3(center.x - a / 2, center.y - a / 2, center.z - a / 2);
    auto p1 = make_double3(p0.x, p0.y, p0.z);
    auto p2 = make_double3(p0.x, p0.y + a, p0.z);
    auto p3 = make_double3(p0.x + a, p0.y + a, p0.z);
    auto p4 = make_double3(p0.x + a, p0.y, p0.z);
    auto p5 = make_double3(p0.x, p0.y, p0.z + a);
    auto p6 = make_double3(p0.x, p0.y + a, p0.z + a);
    auto p7 = make_double3(p0.x + a, p0.y + a, p0.z + a);
    auto p8 = make_double3(p0.x + a, p0.y, p0.z + a);

    polygons[++start] = Polygon(p1, p2, p3, color);
    polygons[++start] = Polygon(p3, p4, p1, color);
    polygons[++start] = Polygon(p7, p8, p4, color);
    polygons[++start] = Polygon(p4, p3, p7, color);
    polygons[++start] = Polygon(p3, p2, p6, color);
    polygons[++start] = Polygon(p6, p7, p3, color);
    polygons[++start] = Polygon(p5, p6, p2, color);
    polygons[++start] = Polygon(p2, p1, p5, color);
    polygons[++start] = Polygon(p4, p8, p5, color);
    polygons[++start] = Polygon(p5, p1, p4, color);
    polygons[++start] = Polygon(p7, p6, p5, color);
    polygons[++start] = Polygon(p5, p8, p7, color);
}

void build_octahedron(Polygon *polygons, double3 center, uchar4 color, double r, int &start) {
    auto p1 = make_double3(center.x, center.y - r, center.z);
    auto p2 = make_double3(center.x - r, center.y, center.z);
    auto p3 = make_double3(center.x, center.y + r, center.z);
    auto p4 = make_double3(center.x + r, center.y, center.z);
    auto p5 = make_double3(center.x, center.y, center.z - r);
    auto p6 = make_double3(center.x, center.y, center.z + r);


    polygons[++start] = Polygon(p1, p2, p5, color);
    polygons[++start] = Polygon(p2, p3, p5, color);
    polygons[++start] = Polygon(p3, p4, p5, color);
    polygons[++start] = Polygon(p4, p1, p5, color);
    polygons[++start] = Polygon(p1, p6, p2, color);
    polygons[++start] = Polygon(p2, p6, p3, color);
    polygons[++start] = Polygon(p3, p6, p4, color);
    polygons[++start] = Polygon(p4, p6, p1, color);
}

double3 read_double3() {
    double3 num;
    std::cin >> num.x >> num.y >> num.z;
    return num;
}

void writeToFile(char *output_dir, int w, int h, int screen_size, uchar4 *data_out) {
	std::ofstream outputFS(output_dir, std::ios::out | std::ios::binary);  
	outputFS.write((char *) &w, sizeof(int));   
	outputFS.write((char *) &h, sizeof(int)); 
	outputFS.write((char *) data_out, screen_size*sizeof(uchar4));  
	outputFS.close();
}

void print_log(int k, float time, long long cnt) {
    std::cout << k << '\t' << time << '\t' << cnt << '\n';
}

void print_default() {
    std::cout << DEFAULT_INPUT;
}

/*
run with:
    '--cpu' to make rendering without using GPU;
    '--default' to get default input;
    '--gpu' \or nothing\  make rendering with using GPU;
*/
int main(int argc, char *argv[]) {
    bool GPU = true;
    if (argc >= 2) {
        auto key = std::string(argv[1]);
        if (key == "--default") {
            print_default();
            return 0;
        }
        GPU = key == "--cpu" ? false : true; 
    }

    int frames; 
    std::cin >> frames;
    
    char output_dir[BUFF_SIZE];
    std::cin >> output_dir;

    int w, h;
    std::cin >> w >> h;
    
    double angle;
    std::cin >> angle;

    Motion_params cam, dir;
    std::cin >> cam >> dir;

    Figure tetrahedron, hexahedron, octahedron;
    std::cin >> tetrahedron >> hexahedron >> octahedron;
    
    double3 floor_coord[4];
    for (int i = 0; i < 4; i++)
        floor_coord[i] = read_double3();

    auto floor_color = read_double3();

    auto light_coord = read_double3();
    auto light_color = read_double3();
    
    double sqrtSamples;
    std::cin >> sqrtSamples;

    auto floor_color_uch4 = make_uchar4(floor_color.x * RGB_CONST, floor_color.y * RGB_CONST, floor_color.z * RGB_CONST, RGB_CONST);
    auto tetrahedron_color = make_uchar4(tetrahedron.color.x * RGB_CONST, tetrahedron.color.y * RGB_CONST, tetrahedron.color.z * RGB_CONST, RGB_CONST);
    auto hexahedron_color = make_uchar4(hexahedron.color.x * RGB_CONST, hexahedron.color.y * RGB_CONST, hexahedron.color.z * RGB_CONST, RGB_CONST);
    auto octahedron_color = make_uchar4(octahedron.color.x * RGB_CONST, octahedron.color.y * RGB_CONST, octahedron.color.z * RGB_CONST, RGB_CONST);

    int start = 0;
    Polygon polygons[POLYGONS_CNT];
    build_space(polygons, floor_coord, floor_color_uch4, start);
    build_tetrahedron(polygons, tetrahedron.center, tetrahedron_color, tetrahedron.radius, start);
    build_hexahedron(polygons, hexahedron.center, hexahedron_color, hexahedron.radius, start);
	build_octahedron(polygons, octahedron.center, octahedron_color, octahedron.radius, start);

    auto lpos = make_double3(light_coord.x, light_coord.y, light_coord.z);
    auto lcol = make_uchar4(light_color.x*RGB_CONST, light_color.y*RGB_CONST, light_color.z*RGB_CONST, RGB_CONST);

    const int screen_size = w*h;
    uchar4 *dev, *dev_out,
        *data_out = (uchar4*) malloc(sizeof(uchar4) * screen_size),
        *data = (uchar4*) malloc(sizeof(uchar4) * screen_size * sqrtSamples * sqrtSamples);
    
    Polygon *dev_polygons;
    
    if (GPU) {
        CSC(cudaMalloc(&dev, sizeof(uchar4)*screen_size*sqrtSamples*sqrtSamples));
        CSC(cudaMalloc(&dev_out, sizeof(uchar4)*screen_size));
        CSC(cudaMalloc(&dev_polygons, sizeof(Polygon)*POLYGONS_CNT));
        CSC(cudaMemcpy(dev_polygons, polygons, sizeof(Polygon)*POLYGONS_CNT, cudaMemcpyHostToDevice));
    }

    int k = 0;
    while (++k <= frames) {
        double t = 2*M_PI*k / frames;
        
        double cam_r = cam.r0 + cam.A*sin(cam.wr*t + cam.pr);
        double cam_z = cam.z0 + cam.Az*sin(cam.wz*t + cam.pz);
        double cam_phi = cam.phi0 + cam.wphi*t;

        double dir_r = dir.r0 + dir.A*sin(dir.wr*t + dir.pr);
        double dir_z = dir.z0 + dir.Az*sin(dir.wz*t + dir.pz);
        double dir_phi = dir.phi0 + dir.wphi*t;

        auto cam_pos = make_double3(cam_r*cos(cam_phi), cam_r*sin(cam_phi), cam_z);
        auto cam_view = make_double3(dir_r*cos(dir_phi), dir_r*sin(dir_phi), dir_z);

        cudaEvent_t start, stop;
        CSC(cudaEventCreate(&start));
        CSC(cudaEventCreate(&stop));
        CSC(cudaEventRecord(start));

        if (GPU) {
            kernel_render<<<dim3(32, 32), dim3(16, 16)>>>(dev, dev_polygons, cam_pos, cam_view, w*sqrtSamples, h*sqrtSamples, angle, lpos, lcol);
            CSC(cudaGetLastError());
            
            kernel_ssaa<<<dim3(32, 32), dim3(16, 16)>>>(dev, dev_out, w, h, sqrtSamples);
            CSC(cudaGetLastError());
            
            CSC(cudaMemcpy(data_out, dev_out, sizeof(uchar4) *screen_size, cudaMemcpyDeviceToHost));
        } else {
            make_cpu_render(data, polygons, cam_pos, cam_view, w * sqrtSamples, h * sqrtSamples, angle, lpos, lcol);
            make_cpu_ssaa(data, data_out, w, h, sqrtSamples);
        }

        CSC(cudaEventRecord(stop));
        CSC(cudaEventSynchronize(stop));
        
        float time;
        CSC(cudaEventElapsedTime(&time, start, stop));
        CSC(cudaEventDestroy(start));
        CSC(cudaEventDestroy(stop));
        
        char buff[BUFF_SIZE];
        sprintf(buff, output_dir, k);
        writeToFile(buff, w, h, w*h, data_out);

        print_log(k, time, screen_size*sqrtSamples*sqrtSamples);
    }

    free(data);
    free(data_out);
    if (GPU) {
        CSC(cudaFree(dev));
        CSC(cudaFree(dev_out));
    }
    return 0;
}

