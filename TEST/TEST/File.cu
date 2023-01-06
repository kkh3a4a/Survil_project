#include <stdio.h>
#include "\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v12.0\include\cuda_runtime.h"

void helloCPU()
{
    printf("Hello from the CPU.\n");
}

__global__ void helloGPU()
{
    for (int i = 0; i < 40000; ++i) {
        for (int j = 0; j < 40000; ++j) {
            printf("%d ", j);
        }
        printf("\n");
    }
}
