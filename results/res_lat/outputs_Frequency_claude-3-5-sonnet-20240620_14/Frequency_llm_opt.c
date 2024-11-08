#include <stdio.h>

#define N 128
int epsilon[N]; // array of 0s and 1s

#define N 128

void Frequency(int *result, int epsilon_element) {
    #pragma hls_pipeline_init_interval 1
    static int sum = 0;
    static int i = 0;

    sum += (2 * ((int) epsilon_element)) - 1;

    if (i == N - 1) {
        *result = sum;
        sum = 0;
        i = 0;
    } else {
        i++;
    }
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    
    int result;
    for (i = 0; i < N; i++) {
        Frequency(&result, epsilon[i]);
    }

    printf("result = %d\n", result);
    return 0;
}
