#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s



#define N 16
#define M 8

// Note: epsilon array is assumed to be defined externally

int BlockFrequency(int epsilon_element) {
    #pragma hls_pipeline_init_interval 1
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static int sum = 0;
    int pi;
    int v;
    const int half = 8192; // 0.5 in Q14 fixed-point format
    int result = 0;

    if (j < 8) {
        #pragma hls_unroll yes
        blockSum += epsilon_element;
        j++;
    }

    if (j == 8) {
        pi = (blockSum << 14) / 8; // Convert to Q14 and divide by 8
        v = pi - half;
        sum += ((long long)v * v) >> 14; // Multiply in Q28 and shift back to Q14
        
        blockSum = 0;
        j = 0;
        i++;
        
        if (i == 16) {
            result = sum;
            sum = 0;
            i = 0;
        }
    }

    return result;
}

int main() {
    int i;
    int epsilon[N*M];
    for (i = 0; i < N*M; i++) {
        epsilon[i] = i*73%7 == 0;
    }
    
    int result = 0;
    int temp_result;
    
    for (i = 0; i < N*M; i++) {
        temp_result = BlockFrequency(epsilon[i]);
        if (temp_result != 0) {
            result = temp_result;
        }
    }

    printf("result = %f\n", (double)result / 16384.0);
    return 0;
}
