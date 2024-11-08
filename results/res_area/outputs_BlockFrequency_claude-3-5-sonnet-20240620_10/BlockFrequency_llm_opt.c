#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#define N 16
#define M 8

// Assuming epsilon is defined elsewhere as per your instructions
extern int epsilon[N*M];

void BlockFrequency(int *result, int epsilon_element) {
    #pragma HLS INLINE off
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static int sum = 0;
    int pi;
    int v;
    const int half = 128; // 0.5 in Q7.8 fixed-point format

    #pragma HLS PIPELINE II=1

    if (j < 8) {
        blockSum += epsilon_element;
        j++;
    }

    if (j == 8) {
        pi = (blockSum << 8) / 8; // Convert to Q7.8 fixed-point
        v = pi - half;
        sum += (v * v) >> 8; // Adjust back to Q7.8 after multiplication
        
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

// Main function for testing
int main() {
    int i;
    for (i = 0; i < N*M; i++) {
        epsilon[i] = i*73%7 == 0;
    }
    
    int result = 0;
    
    for (i = 0; i < N*M; i++) {
        BlockFrequency(&result, epsilon[i]);
    }

    printf("result = %f\n", result / 256.0); // Convert back to floating-point for display
    
    return 0;
}
