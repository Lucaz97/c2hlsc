#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

void Runs(int *res_S, int *res_V) {
    static int S = 0;
    static int V = 1;
    static int prev_epsilon = 0;

    #pragma hls_pipeline_init_interval 1
    for (int i = 0; i < N; i++) {
        int epsilon_element = epsilon[i];

        if (i == 0) {
            S = 0;
            V = 1;
            prev_epsilon = epsilon_element;
            if (epsilon_element) {
                S++;
            }
        } else {
            if (epsilon_element) {
                S++;
            }
            if (epsilon_element != prev_epsilon) {
                V++;
            }
            prev_epsilon = epsilon_element;
        }
    }

    #pragma hls_unroll yes
    if (1) {
        *res_S = S;
        *res_V = V;
        S = 0;
        V = 1;
        prev_epsilon = 0;
    }
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S, result_V;
    Runs(&result_S, &result_V);

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V);  // Corrected this line

    return 0;
}
