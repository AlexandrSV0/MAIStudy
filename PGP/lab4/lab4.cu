#include <iostream>
#include <thrust/extrema.h>
#include <thrust/device_vector.h>
#include <math.h>

using namespace std;

#define FastIO ios_base::sync_with_stdio(false); cin.tie(nullptr), cout.tie(nullptr); 
#define CATCH(call)                                                            \
    do {                                                                                 \
        cudaError_t res = call;                                                 \
        if (res != cudaSuccess)                                   			   \
        {                                                          					        \
            fprintf(stderr, "ERROR in %s:%d. Message: %s\n",  \
                    __FILE__, __LINE__, cudaGetErrorString(res)); 	 \
            exit(0);                                              			   			  \
        }                                                        							 \
    } while (0);

struct comparator {												
    __host__ __device__ bool operator()(double a, double b) { 
        return abs(a) < abs(b); 
    } 
}; 

__host__ void readMatrixAndB(double* matrix, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            std::cin >> matrix[j*n + i];
    
    for (int i = 0; i < n; i++)
        std::cin >> matrix[n*n + i];
}

__global__ void swapRows(double* data, int n, int curRow, int rowToSwap) {
     int id_x = blockDim.x*blockIdx.x + threadIdx.x; 
    for (int j = id_x; j < n+1; j += blockDim.x*gridDim.x) { 
        double cp = data[j*n + curRow]; 
        data[j*n + curRow] = data[j*n+ rowToSwap]; 
        data[j*n + rowToSwap] = cp; 
    }
}

__global__ void Gauss(double* data, int n, int row) { 
    int id_x = blockDim.x*blockIdx.x + threadIdx.x; 
    int id_y = blockDim.y*blockIdx.y + threadIdx.y; 
    int off_x = blockDim.x*gridDim.x; 
    int off_y = blockDim.y*gridDim.y; 
    
    int column = row;
    for (int i = id_x + row + 1; i < n; i+= off_x)
        for (int j = id_y + column+1; j < n+1; j+= off_y)
            data[j*n + i] += data[j*n + row] * (-data[column*n + i] / data[column*n + row]); 
}

__host__ void solveEquatation(double* matrix, double* res, int n) {
    for (int i = n-1; i >= 0; i--) { 
        double b_i = matrix[n*n + i];
        for (int j = n - 1; j > i; j--)
            b_i -= res[j] * matrix[j*n + i];
        res[i] = b_i / matrix[i*n + i];
    }
}

int main() {
    FastIO
    int n;
    cin >> n;
    int matrixSize = n*n + n;
    double *matrix = new double[matrixSize];
    readMatrixAndB(matrix, n);

    double* dataDev; 
    CATCH(cudaMalloc(&dataDev, sizeof(double)*matrixSize));
    CATCH(cudaMemcpy(dataDev, matrix, sizeof(double)*matrixSize, cudaMemcpyHostToDevice));

    cudaEvent_t start, stop;
    CATCH(cudaEventCreate(&start));
    CATCH(cudaEventCreate(&stop));
    CATCH(cudaEventRecord(start));
    comparator comparator; 
    for (int j = 0; j < n-1; j++) { 
        //ptr на столбец j 
        auto columnPtr = thrust::device_pointer_cast(dataDev + j*n);	 
        // ptr на max el (в столбце по строкам от i до n) 
        int i = j; 
        auto maxPtr = thrust::max_element(columnPtr + i,  columnPtr + n, comparator);	
        int maxPos = maxPtr - columnPtr; 
        if (maxPos != i) swapRows<<<256, 256>>>(dataDev, n, i, maxPos); 

        Gauss<<<dim3(64, 64), dim3(32, 32)>>>(dataDev, n, i); 
    }

    CATCH(cudaEventRecord(stop));
    CATCH(cudaEventSynchronize(stop));

    float t;
    CATCH(cudaEventElapsedTime(&t, start, stop));
    CATCH(cudaEventDestroy(start));
    CATCH(cudaEventDestroy(stop));

    printf("time = %f ms\n", t);

    CATCH(cudaMemcpy(matrix, dataDev, sizeof(double)*matrixSize, cudaMemcpyDeviceToHost));
    CATCH(cudaFree(dataDev)); 

    double* res = new double[n];
    solveEquatation(matrix, res, n);
  
    // for (int i = 0; i < n; i++)
    //     printf("%10e ", res[i]);

    free(matrix); 	
    free(res); 
}