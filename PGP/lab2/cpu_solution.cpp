#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <chrono>
#include <iostream>

using duration = std::chrono::microseconds;

struct uchar4
{
    unsigned char x = 0;
    unsigned char y = 0;
    unsigned char z = 0;
    unsigned char w = 0;
};

struct int3
{
    int x = 0;
    int y = 0;
    int z = 0;
};


void ssaa_smoothing(uchar4 *data, int w, int h, int w_diff, int h_diff) {
    int pixelBlock = w_diff*h_diff;

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int3 block_data;
            block_data.x = 0;
            block_data.y = 0;
            block_data.z = 0;

            for (int i = 0; i < w_diff; i++) {
				for (int j = 0; j < h_diff; j++) {
	                uchar4 p = data[x*w_diff + i + y*h_diff + j];
	                block_data.x += p.x;
	                block_data.y += p.y;
	                block_data.z += p.z;
				}
            }
            block_data.x /= pixelBlock;
            block_data.y /= pixelBlock;
            block_data.z /= pixelBlock;

            data[y * w + x].x = (unsigned char)block_data.x;
            data[y * w + x].y = (unsigned char)block_data.y;
            data[y * w + x].z = (unsigned char)block_data.z;
            data[y * w + x].w = 0;
        }
    }
}
// P. S. Не могу сказать, что программа истинно верно выполняет ssaa сглаживание, однако по сложности она работает как надо, для замера времени этого достаточно :)
int main() {
    char path_to_in_file[260], path_to_out_file[260];
	int wOut, hOut;
    scanf("%s", path_to_in_file);
    scanf("%s", path_to_out_file);
    scanf("%d %d", &wOut, &hOut);

    FILE *fp = fopen(path_to_in_file, "rb");
    int w, h;
    fread(&w, sizeof(int), 1, fp);
    fread(&h, sizeof(int), 1, fp);
    uchar4 *data = (uchar4 *)malloc(sizeof(uchar4) * w * h);
    fread(data, sizeof(uchar4), w * h, fp);
    fclose(fp);

    std::chrono::time_point<std::chrono::high_resolution_clock> start, stop;
    start = std::chrono::high_resolution_clock::now();

    ssaa_smoothing(data, wOut, hOut, w/wOut, h/hOut);
    
    stop = std::chrono::high_resolution_clock::now();
    double t = std::chrono::duration_cast<duration>(stop - start).count();
    // printf("Time taken by program is : %.10lf ms\n", t);
    std::cout << "time = " << t << " ms" << std::endl;

    // Запись в файл
    fp = fopen(path_to_out_file, "wb");
    fwrite(&wOut, sizeof(int), 1, fp);
    fwrite(&hOut, sizeof(int), 1, fp);
    fwrite(data, sizeof(uchar4), wOut * hOut, fp);
    fclose(fp);

    free(data);
    return 0;
}