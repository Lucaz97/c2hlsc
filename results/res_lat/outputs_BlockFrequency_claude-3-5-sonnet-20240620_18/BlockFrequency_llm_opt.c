#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#define N 16
#define M 8

// Fixed-point representation: 16 bits for integer part, 16 bits for fractional part
typedef int fixed_t;

#define FRAC_BITS 16
#define TO_FIXED(x) ((fixed_t)((x) * (1 << FRAC_BITS)))
#define FROM_FIXED(x) ((double)(x) / (1 << FRAC_BITS))

void BlockFrequency(fixed_t *result, int epsilon_element) {
    #pragma hls_pipeline_init_interval 1
    static int i = 0;
    static int j = 0;
    static int blockSum = 0;
    static fixed_t sum = 0;
    fixed_t pi;
    fixed_t v;
    const fixed_t half = TO_FIXED(0.5);

    #pragma hls_unroll yes
    if (j < 8) {
        blockSum += epsilon_element;
        j++;
    }

    if (j == 8) {
        pi = (blockSum << (FRAC_BITS - 3)); // Equivalent to dividing by 8
        v = pi - half;
        sum += ((long long)v * v) >> FRAC_BITS;
        
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

int main() {
    int i;
    int epsilon[N*M];
    for (i = 0; i < N*M; i++) {
        epsilon[i] = i*73%7 == 0;
    }
    
    fixed_t result = 0;
    
    for (i = 0; i < N*M; i++) {
        BlockFrequency(&result, epsilon[i]);
    }

    printf("result = %f\n", FROM_FIXED(result));
    
    return 0;
}
