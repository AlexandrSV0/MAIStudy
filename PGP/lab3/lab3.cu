#include <iostream>
#include <fstream>
#include <vector>

#define CSC(call)                                                                \
    do {                                                                                 \
        cudaError_t res = call;                                                 \
        if (res != cudaSuccess)                                   			   \
        {                                                          					        \
            fprintf(stderr, "ERROR in %s:%d. Message: %s\n",  \
                    __FILE__, __LINE__, cudaGetErrorString(res)); 	 \
            exit(0);                                              			   			  \
        }                                                        							 \
    } while (0);

const float FLOAT_MIN = -1.7976931348623158e+308;
//добавил константую память
__constant__ float3 avg_dev[32];
__constant__ float3 covInv_dev[32*3]; 
__constant__ float covDet_modif_dev[32]; 
 

// avg
__host__ void calculateAvg(float3 *avg, uchar4 *data, std::vector<std::vector<int2>> classes, int w) {
    int classCount = classes.size();
    for (int i = 0; i < classCount; i++) {
       avg[i] = make_float3(0.f, 0.f, 0.f);

        for (int j = 0; j < classes[i].size(); j++) {
            int2 coord = classes[i][j];
            uchar4 pix = data[coord.x + coord.y*w];

            avg[i].x += pix.x; 
            avg[i].y += pix.y; 
            avg[i].z += pix.z; 
        }

        int classSize = classes[i].size();
        avg[i].x /= classSize; 
        avg[i].y /= classSize;  
        avg[i].z /= classSize; 
    }
}

// cov
__host__ void calculateCov(float3 (*cov)[3], uchar4 *data, float3 *avg, std::vector<std::vector<int2>> classes, int w) {
    int classCount = classes.size();

    for (int i = 0; i < classCount; i++) {
        for (int k = 0; k < 3; k++) {
            cov[i][k] = make_float3(0.f, 0.f, 0.f);
        }

        float3 rgb_avg = avg[i];
        int classSize = classes[i].size();

        for (int j = 0; j < classSize; j++) {
            int2 coord = classes[i][j];
            uchar4 pix = data[coord.x + coord.y * w];

            for (int k = 0; k < 3; k++) {
                float diffX = pix.x - rgb_avg.x;
                float diffY = pix.y - rgb_avg.y; 
                float diffZ = pix.z - rgb_avg.z; 

                cov[i][k].x += diffX * ((k == 0) ? diffX : (k == 1) ? diffY : diffZ); 
                cov[i][k].y += diffY * ((k == 0) ? diffX : (k == 1) ? diffY : diffZ); 
                cov[i][k].z += diffZ * ((k == 0) ? diffX : (k == 1) ? diffY : diffZ); 
            }  
        }

        for (int k = 0; k < 3; k++) {
            cov[i][k].x /= classSize-1;
            cov[i][k].y /= classSize-1;
            cov[i][k].z /= classSize-1;
        }
    }
}

// det(cov)
__host__ void calculateCovDet(float *covDet, float3 (*cov)[3], int classCount) {
    for (int i = 0;  i < classCount; i++) {
        float term1 = cov[i][0].x * cov[i][1].y * cov[i][2].z;
        float term2 = cov[i][0].y * cov[i][1].z * cov[i][2].x; 
        float term3 = cov[i][0].z * cov[i][1].x * cov[i][2].y; 
        float term4 = cov[i][0].z * cov[i][1].y * cov[i][2].x;  
        float term5 = cov[i][0].y * cov[i][1].x * cov[i][2].z; 
        float term6 = cov[i][0].x * cov[i][1].z * cov[i][2].y;

        covDet[i] = term1 + term2 + term3 - term4 - term5 - term6; 
    }
}

// cov^(-1)
__host__ void calculateCovInv(float3 *covInv, float3 (*cov)[3], float *covDet, int classCount) {
    for (int i = 0; i < classCount; i++) {
        int j = i * 3;
        float det = covDet[i];

        for (int k = 0; k < 3; k++) {
            covInv[j + k].x = (k == 0) ? cov[i][1].y * cov[i][2].z - cov[i][1].z * cov[i][2].y 
                                        : (k == 1) ? -(cov[i][0].y * cov[i][2].z - cov[i][0].z * cov[i][2].y) 
                                        : cov[i][0].y * cov[i][1].z - cov[i][0].z * cov[i][1].y; 

            covInv[j + k].y = (k == 0) ? -(cov[i][1].x * cov[i][2].z - cov[i][1].z * cov[i][2].x)
                                        : (k == 1) ? cov[i][0].x * cov[i][2].z - cov[i][0].z * cov[i][2].x
                                        : -(cov[i][0].x * cov[i][1].z - cov[i][0].z * cov[i][1].x);  

            covInv[j + k].z = (k == 0) ? cov[i][1].x * cov[i][2].y - cov[i][1].y * cov[i][2].x 
                                        : (k == 1) ? -(cov[i][0].x * cov[i][2].y - cov[i][0].y * cov[i][2].x) 
                                        : cov[i][0].x * cov[i][1].y - cov[i][0].y * cov[i][1].x; 
 
            covInv[j + k].x /= det;
            covInv[j + k].y /= det;
            covInv[j + k].z /= det;
        }
    }
}

__device__ float D_f(uchar4 pix, int j) {
    int i = j/3;
    float px_avgx = pix.x - avg_dev[i].x; 
    float py_avgy = pix.y - avg_dev[i].y; 
    float pz_avgz = pix.z - avg_dev[i].z; 

    float summand_1 = (px_avgx*covInv_dev[j].x + py_avgy*covInv_dev[j+1].x + pz_avgz*covInv_dev[j+2].x)*px_avgx; 
    float summand_2 = (px_avgx*covInv_dev[j].y + py_avgy*covInv_dev[j+1].y + pz_avgz*covInv_dev[j+2].y)*py_avgy; 
    float summand_3 = (px_avgx*covInv_dev[j].z + py_avgy*covInv_dev[j+1].z + pz_avgz*covInv_dev[j+2].z)*pz_avgz; 
    
    return -(summand_1 + summand_2 + summand_3) - covDet_modif_dev[i];
}

// ММП
__device__ int predictPixelClass(int classCount, uchar4 pix) {
    float D_max = FLOAT_MIN;
    int classNum = 0;
    
    for (int i = 0; i < classCount; i++) {
        float D_cur = D_f(pix, i*3);

        if (D_cur > D_max) {
            D_max = D_cur;
            classNum = i;
        }
    }
    return classNum;
}

__global__ void kernel(uchar4 *data, int img_size, int classCount) {
    // поменял на одномерную сетку потоков здесь и в 216 строке; не обратил внимание на цель работы
    int id_x = blockDim.x * blockIdx.x + threadIdx.x;
    int offset_x = blockDim.x * gridDim.x;

    for (int i = id_x; i < img_size; i += offset_x) {    
        data[i].w = (unsigned char) predictPixelClass(classCount, data[i]); 
    }
}

__host__ void printFloat3(float3 num) {
    printf("f3[%f | %f | %f]", num.x, num.y, num.z); 
}

int main() {
    std::string inputFile, outputFile;
    std::cin >> inputFile >> outputFile;

    int w, h;
    std::ifstream inputFS(inputFile, std::ios::in | std::ios::binary);
    inputFS.read((char *) &w, sizeof(w));
    inputFS.read((char *) &h, sizeof(h));  
    int img_size = w*h; 
    uchar4 *data = new uchar4[img_size]; 
    inputFS.read((char *) data, img_size*sizeof(uchar4));  
    inputFS.close(); 

    int classCount; 
    std::cin >> classCount;
    std::vector<std::vector<int2>> classes;

    for (int i = 0; i < classCount; i++) { 
        int classSize; 
        std::cin >> classSize; 
        std::vector<int2> curClass(classSize); 
        classes.push_back(curClass);

        for (int j = 0; j < classSize; j++) {
            int2 coord; 
            std::cin >> coord.x >> coord.y;
            classes[i][j] = coord; 
        } 
    } 

    float3 avg_host[classCount]; 
    calculateAvg(avg_host, data, classes, w); 
    
    float3 cov_host[classCount][3];
    calculateCov(cov_host, data, avg_host, classes, w); 

    float covDet_host[classCount];  
    calculateCovDet(covDet_host, cov_host, classCount);

    float covDetModified_host[classCount];
    for (int i = 0; i < classCount; i++) 
        covDetModified_host[i] = log(abs(covDet_host[i])); 

    float3 covInv_host[classCount*3];
    calculateCovInv(covInv_host, cov_host, covDet_host, classCount);
 
    uchar4 *data_dev; 
    CSC(cudaMalloc(&data_dev, sizeof(uchar4)*img_size));
    CSC(cudaMemcpy(data_dev, data, sizeof(uchar4) * img_size, cudaMemcpyHostToDevice)); 

    CSC(cudaMemcpyToSymbol(avg_dev, avg_host, sizeof(float3)*classCount,0, cudaMemcpyHostToDevice)); 
    CSC(cudaMemcpyToSymbol(covInv_dev, covInv_host, sizeof(float3)*classCount*3,0, cudaMemcpyHostToDevice)); 
    CSC(cudaMemcpyToSymbol(covDet_modif_dev, covDetModified_host, sizeof(float)*classCount,0, cudaMemcpyHostToDevice)); 
 
    cudaEvent_t start, stop;
    CSC(cudaEventCreate(&start));
    CSC(cudaEventCreate(&stop));
    CSC(cudaEventRecord(start));

    kernel<<<1024, 1024>>>(data_dev, img_size, classCount); 
    CSC(cudaGetLastError());

    CSC(cudaEventRecord(stop));
    CSC(cudaEventSynchronize(stop));

    float t;
    CSC(cudaEventElapsedTime(&t, start, stop));
    CSC(cudaEventDestroy(start));
    CSC(cudaEventDestroy(stop));

    printf("time = %f ms\n", t);

    CSC(cudaMemcpy(data, data_dev, sizeof(uchar4)*img_size, cudaMemcpyDeviceToHost)); 
    
    CSC(cudaFree(data_dev));

	std::ofstream outputFS(outputFile, std::ios::out | std::ios::binary);  
	outputFS.write((char *) &w, sizeof(int));   
	outputFS.write((char *) &h, sizeof(int)); 
	outputFS.write((char *) data, img_size*sizeof(uchar4));  
	outputFS.close();
    free(data);
}