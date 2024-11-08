#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#define N 16
#define M 8

void BlockFrequency(int *result, int epsilon_element) {
    #pragma HLS INLINE off
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static int sum = 0;

    #pragma HLS PIPELINE II=1

    if (j < 8) {
        blockSum += epsilon_element;
        j++;
    }

    if (j == 8) {
        // Using integer arithmetic: multiply by 1024 to preserve precision (2^10)
        int pi = (blockSum * 1024) / 8;
        int v = pi - 512;  // 512 represents 0.5 * 1024
        sum += (v * v) / 1024;  // Divide by 1024 to adjust for the scaling
        
        blockSum = 0;
        j = 0;
        i++;
    }

    if (i == 16) {
        *result = sum;
        sum = 0;
        i = 0;
    }
}

// The epsilon array is assumed to be defined externally

int main() {
    int i;
    int epsilon[N*M];
    for (i = 0; i < N*M; i++) {
        epsilon[i] = i*73%7 == 0;
    }
    
    int result = 0;
    
    for (i = 0; i < N*M; i++) {
        BlockFrequency(&result, epsilon[i]);
    }

    printf("result = %f\n", (float)result / 1024.0);  // Convert back to float for display
    return 0;
}
