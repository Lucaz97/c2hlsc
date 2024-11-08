#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#define N 16
#define M 8

// Using int for fixed-point representation
// We'll use 16 bits for the fractional part
#define FIXED_POINT_FRACTIONAL_BITS 16

int BlockFrequency(int epsilon_element) {
    #pragma hls_pipeline_init_interval 1
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static int sum = 0;
    int pi;
    int v;
    const int half = 1 << (FIXED_POINT_FRACTIONAL_BITS - 1); // 0.5 in fixed-point

    #pragma hls_unroll yes
    if (j < 8) {
        blockSum += epsilon_element;
        j++;
    }

    if (j == 8) {
        // pi = blockSum / 8 (in fixed-point)
        pi = (blockSum << FIXED_POINT_FRACTIONAL_BITS) / 8;
        v = pi - half;
        // Multiply v * v and accumulate
        sum += (int)(((long long)v * v) >> FIXED_POINT_FRACTIONAL_BITS);
        
        blockSum = 0;
        j = 0;
        i++;
    }

    int result = 0;
    if (i == 16) {
        result = sum;
        sum = 0;
        i = 0;
    }

    return result;
}

// The main function needs to be adjusted accordingly
int main() {
    int i;
    int epsilon[N*M];
    for (i = 0; i < N*M; i++) {
        epsilon[i] = i*73%7 == 0;
    }
    
    int result = 0;
    
    for (i = 0; i < N*M; i++) {
        result += BlockFrequency(epsilon[i]);
    }

    // Convert fixed-point to float for printing
    float float_result = (float)result / (1 << FIXED_POINT_FRACTIONAL_BITS);
    printf("result = %f\n", float_result);
    
    return 0;
}
