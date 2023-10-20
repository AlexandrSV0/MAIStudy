#include <iostream>
#include <fstream>


#define CATCH_ERR(call) \
do { \
	cudaError_t res = call;	\
	if (res != cudaSuccess) {										\
		fprintf(stderr, "ERROR in %s:%d. Message: %s\n", \
				__FILE__, __LINE__, cudaGetErrorString(res)); \
		exit(0); \
	} \
} while(0) \


typedef struct {
    int w;
    int h;
} ImageSize;


__global__ void ssaa_smoothing(cudaTextureObject_t tex, uchar4 *dev, const ImageSize out_img, int w_diff, int h_diff) {
    int pixelBlock = w_diff*h_diff;
    int id_x = blockDim.x*blockIdx.x + threadIdx.x;
    int id_y = blockDim.y*blockIdx.y + threadIdx.y;
    int offset_x = blockDim.x*gridDim.x;
    int offset_y = blockDim.y*gridDim.y;

    for (int x = id_x; x < out_img.w; x += offset_x) {
        for (int y = id_y; y < out_img.h; y += offset_y) {
            int3 block_data;
            block_data.x = 0;
            block_data.y = 0;
            block_data.z = 0;
            int alpha;

            for (int i = 0; i < w_diff; i++) {
                for (int j = 0; j < h_diff; j++) {
                    uchar4 pix = tex2D<uchar4>(tex,  x*w_diff + i, y*h_diff + j);
                    block_data.x += pix.x;
                    block_data.y += pix.y;
                    block_data.z += pix.z;
                    alpha = pix.w;
                }
            }

            block_data.x /= pixelBlock;
            block_data.y /= pixelBlock;
            block_data.z /= pixelBlock;
            dev[y*out_img.w + x] = make_uchar4(block_data.x, block_data.y, block_data.z, alpha);
        }
    }
}


int main() {
	ImageSize out_img;
	int w, h;
	std::string file_in, file_out;
	std::cin >> file_in >> file_out;
	std::cin >> out_img.w >> out_img.h;
    
	std::ifstream fs_in(file_in, std::ios::in | std::ios::binary);
	fs_in.read((char *)&w, sizeof(w));
	fs_in.read((char *)&h, sizeof(h));
	int img_size = w*h;
	int out_img_size = out_img.w*out_img.h;
	uchar4 *data = new uchar4[img_size];
	fs_in.read((char *)data, w*h*sizeof(data[0]));
    fs_in.close();
	
	cudaArray *arr;
	cudaChannelFormatDesc ch = cudaCreateChannelDesc<uchar4>();
	CATCH_ERR(cudaMallocArray(&arr, &ch, w, h));
	CATCH_ERR(cudaMemcpy2DToArray(arr, 0, 0, data, w*sizeof(uchar4), w*sizeof(uchar4), h, cudaMemcpyHostToDevice));

	struct cudaResourceDesc resDesc;
    memset(&resDesc, 0, sizeof(resDesc));
    resDesc.resType = cudaResourceTypeArray;
    resDesc.res.array.array = arr;

    struct cudaTextureDesc texDesc;
    memset(&texDesc, 0, sizeof(texDesc));
    texDesc.addressMode[0] = cudaAddressModeClamp;
    texDesc.addressMode[1] = cudaAddressModeClamp;
    texDesc.filterMode = cudaFilterModePoint;
    texDesc.readMode = cudaReadModeElementType;
    texDesc.normalizedCoords = false;

    cudaTextureObject_t tex = 0;
    CATCH_ERR(cudaCreateTextureObject(&tex, &resDesc, &texDesc, NULL));

	uchar4 *dev;
	CATCH_ERR(cudaMalloc(&dev, sizeof(uchar4)*out_img_size));
	
    cudaEvent_t start, stop;  
    CATCH_ERR(cudaEventCreate(&start));
    CATCH_ERR(cudaEventCreate(&stop));
    CATCH_ERR(cudaEventRecord(start));

    ssaa_smoothing<<<dim3(64, 64), dim3(32, 32)>>>(tex, dev, out_img, w/out_img.w, h/out_img.h);

    CATCH_ERR(cudaDeviceSynchronize());
    CATCH_ERR(cudaGetLastError());

    CATCH_ERR(cudaEventRecord(stop));
    CATCH_ERR(cudaEventSynchronize(stop));
    float t;
    CATCH_ERR(cudaEventElapsedTime(&t, start, stop));
    CATCH_ERR(cudaEventDestroy(start));
    CATCH_ERR(cudaEventDestroy(stop));
    printf("time = %f ms\n", t);

	CATCH_ERR(cudaGetLastError());
    CATCH_ERR(cudaMemcpy(data, dev, sizeof(uchar4)*out_img_size, cudaMemcpyDeviceToHost));

	std::ofstream fs_out(file_out, std::ios::out | std::ios::binary);
	fs_out.write((char *)&out_img.w, sizeof(out_img.w));
	fs_out.write((char *)&out_img.h, sizeof(out_img.h));
	fs_out.write((char *)data, out_img_size*sizeof(data[0])); 
	fs_out.close();

	CATCH_ERR(cudaDestroyTextureObject(tex));
	CATCH_ERR(cudaFreeArray(arr));
	CATCH_ERR(cudaFree(dev));
	free(data);
	return 0;    
}