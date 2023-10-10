#include <stdio.h>

#define CSC(call) 							\
do { 										\
	cudaError_t status = call;				\
	if (status != cudaSuccess) {																				\
		fprintf(stderr, "ERROR is %s:%d. Message: %s\n", __FILE__, __LINE__, cudaGetErrorString(status));		\
		exit(0);								\
	}											\
} while(0)

__global__ void findElByElMaximums(double* vec1, double* vec2, double* res, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int offset = gridDim.x * blockDim.x;
    while (i < n) {
        res[i] = vec1[i] > vec2[i] ? vec1[i] : vec2[i];
        i += offset;
    }
}

int main() {
    int n;
    scanf("%d", &n);

    double *vec1 = (double *) malloc(sizeof(double) * n);
    double *vec2 = (double *) malloc(sizeof(double) * n);
    double *res = (double *) malloc(sizeof(double) * n);

    for (int i = 0; i < n; i++) {
        scanf("%lf", &vec1[i]);
    }
    
    for (int i = 0; i < n; i++) {
        scanf("%lf", &vec2[i]);
    }

    double *dev1, *dev2, *dev_res;
    cudaMalloc(&dev1, sizeof(double) * n);
    cudaMalloc(&dev2, sizeof(double) * n);
    cudaMalloc(&dev_res, sizeof(double) * n);
    cudaMemcpy(dev1, vec1, sizeof(double) * n, cudaMemcpyHostToDevice);
    cudaMemcpy(dev2, vec2, sizeof(double) * n, cudaMemcpyHostToDevice);


    cudaEvent_t start, stop;  
    CSC(cudaEventCreate(&start));
    CSC(cudaEventCreate(&stop));
    CSC(cudaEventRecord(start));

    findElByElMaximums<<<1024, 1024>>>(dev1, dev2, dev_res, n);

    CSC(cudaDeviceSynchronize());
    CSC(cudaGetLastError());

    CSC(cudaEventRecord(stop));
    CSC(cudaEventSynchronize(stop));
    float t;
    CSC(cudaEventElapsedTime(&t, start, stop));
    CSC(cudaEventDestroy(start));
    CSC(cudaEventDestroy(stop));
	
    printf("time = %f ms\n", t);

    cudaMemcpy(res, dev_res, sizeof(double) * n, cudaMemcpyDeviceToHost);

    // for (int i = 0; i < n; i++) {
    //     printf("%.10e ", res[i]);
    // }
    
    cudaFree(dev1);
    cudaFree(dev2);
    cudaFree(dev_res);
    free(vec1);
    free(vec2);
    free(res);
    return 0;
}