#include <stdio.h>

#define N 128
int epsilon[N]; // array of 0s and 1s
#include <stdio.h>

#define N 128

void Frequency(int input[N], int *result) {
#pragma HLS INLINE
    int sum = 0;
    
    for (int i = 0; i < N; i++) {
#pragma HLS UNROLL
        sum += (input[i] != 0) ? 1 : -1;
    }

    *result = sum;
}

int main() {
    int epsilon[N];
    for (int i = 0; i < N; i++) {
        epsilon[i] = (i * 73 % 7 == 0);
    }

    int result;
    Frequency(epsilon, &result);
    
    printf("result = %d\n", result);

    return 0;
}
