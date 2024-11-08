#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#define N 16
#define M 8

extern int epsilon[N*M]; // Declare the external array

// Use integer arithmetic with scaling factor
#define SCALE_FACTOR 1000000

void BlockFrequency(int *result) {
    #pragma HLS INLINE off
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static long long sum = 0;
    int pi;
    int v;
    const int half = SCALE_FACTOR / 2;

    #pragma HLS PIPELINE II=1

    for (int k = 0; k < N*M; k++) {
        #pragma HLS LOOP_TRIPCOUNT min=N*M max=N*M
        int epsilon_element = epsilon[k];

        if (j < M) {
            blockSum += epsilon_element;
            j++;
        }

        if (j == M) {
            pi = (blockSum * SCALE_FACTOR) / M;
            v = pi - half;
            sum += ((long long)v * v) / SCALE_FACTOR;
            
            blockSum = 0;
            j = 0;
            i++;
            
            if (i == N) {
                *result = (int)sum;
                sum = 0;
                i = 0;
            }
        }
    }
}

int main() {
    int i;
    for (i = 0; i < N*M; i++) {
        epsilon[i] = i*73%7 == 0;
    }
    
    int result = 0;
    
    BlockFrequency(&result);

    printf("result = %f\n", (float)result / SCALE_FACTOR);
    
    return 0;
}
