#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s



void BlockFrequency(int *result, int epsilon_element) {
    #pragma HLS PIPELINE II=1
    
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static long long sum = 0; // Use long long for higher precision
    int pi;
    int v;
    const int half = 4; // 0.5 * 8 (scaled by 8)

    #pragma HLS UNROLL factor=8
    if (j < 8) {
        blockSum += epsilon_element;
        j++;
    }

    if (j == 8) {
        pi = blockSum; // Already scaled by 8
        v = pi - half;
        sum += (long long)v * v;
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

// The epsilon array is already defined in the provided code
extern int epsilon[]; // Declaration of the external array

int main() {
    int i;
    for (i = 0; i < 128; i++) {
        epsilon[i] = i*73%7 == 0;
    }
    
    int result = 0;
    
    for (i = 0; i < 128; i++) {
        BlockFrequency(&result, epsilon[i]);
    }

    printf("result = %f\n", (float)result / 64);
    
    return 0;
}
