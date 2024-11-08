#include <stdio.h>

#define N 128
int epsilon[N]; // array of 0s and 1s

#define N 128
// Note: epsilon array is already defined externally

#pragma hls_design top
void Frequency(int *result) {
    static int sum = 0;
    static int i = 0;

    #pragma hls_pipeline_init_interval 1
    for (int j = 0; j < N; j++) {
        int epsilon_element = epsilon[j];
        sum += (2 * ((int) epsilon_element)) - 1;

        if (j == N - 1) {
            *result = sum;
            sum = 0;
            i = 0;
        } else {
            i++;
        }
    }
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result;
    Frequency(&result);

    printf("result = %d\n", result);
    return 0;
}
