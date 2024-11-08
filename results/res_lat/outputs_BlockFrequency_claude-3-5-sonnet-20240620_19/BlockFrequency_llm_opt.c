#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s



#define SCALE_FACTOR 1024

void BlockFrequency(int *result, int epsilon_element) {
    #pragma hls_pipeline_init_interval 1
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static int sum = 0;
    int pi;
    int v;
    const int half = SCALE_FACTOR / 2;

    if (j < 8) {
        #pragma hls_unroll yes
        blockSum += epsilon_element;
        j++;
    }

    if (j == 8) {
        pi = (blockSum * SCALE_FACTOR) / 8;
        v = pi - half;
        sum += (v * v) / SCALE_FACTOR;
        
        blockSum = 0;
        j = 0;
        i++;
        
        if (i == 16) {
            *result = sum;
            sum = 0;
            i = 0;
        }
    }
}

#define N 16
#define M 8
// The epsilon array is already defined in the provided code

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

    printf("result = %f\n", (float)result / SCALE_FACTOR);
    
    return 0;
}
