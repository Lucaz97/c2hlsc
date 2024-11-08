#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#define N 16
#define M 8

extern int epsilon[N*M]; // Declare the epsilon array as external

// Use integer arithmetic with scaling factor
#define SCALE_FACTOR 1000000

void BlockFrequency(int *result) {
    #pragma hls_pipeline_init_interval 1
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static long long sum = 0; // Use long long for larger precision
    int pi;
    int v;
    const int half = SCALE_FACTOR / 2;

    #pragma hls_unroll yes
    for (int k = 0; k < 8; k++) {
        blockSum += epsilon[i*8 + k];
        j++;
    }

    if (j == 8) {
        pi = (blockSum * SCALE_FACTOR) / 8;
        v = pi - half;
        sum += ((long long)v * v) / SCALE_FACTOR;
        
        blockSum = 0;
        j = 0;
        i++;
    }

    if (i == 16) {
        *result = (int)sum;
        sum = 0;
        i = 0;
    }
}

int main() {
    int i;
    for (i = 0; i < N*M; i++) {
        epsilon[i] = i*73%7 == 0;
    }
    int result = 0;
    
    for (i = 0; i < N; i++) {
        BlockFrequency(&result);
    }

    printf("result = %f\n", (double)result / SCALE_FACTOR);
    return 0;
}
