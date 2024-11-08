#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s

#pragma hls_design top
void Runs(int *res_S, int *res_V, int epsilon_element) {
    #pragma hls_pipeline_init_interval 1
    static int S = 0;
    static int V = 1;
    static int k = 0;
    static int prev_epsilon = 0;

    if (k == 0) {
        S = 0;
        V = 1;
        prev_epsilon = epsilon_element;
    }

    if (epsilon_element) {
        S++;
    }

    if (k > 0 && epsilon_element != prev_epsilon) {
        V++;
    }

    prev_epsilon = epsilon_element;
    k++;

    if (k == 65535) {
        *res_S = S;
        *res_V = V;
        k = 0;
    } else {
        *res_S = -1;  // Indicate that the result is not ready
        *res_V = -1;
    }
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    
    int result_S, result_V;
    
    #pragma hls_pipeline_init_interval 1
    for (i = 0; i < N; i++) {
        Runs(&result_S, &result_V, epsilon[i]);
    }

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V);  // Corrected this line
    
    return 0;
}
