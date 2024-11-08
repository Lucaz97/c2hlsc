#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

void Runs(int *res_S, int *res_V) {
    static int S = 0;
    static int V = 1;
    static int prev_epsilon = 0;
    static int k = 0;

    #pragma hls_pipeline_init_interval 1
    for (; k < N; k++) {
        int epsilon_element = epsilon[k];
        
        if (k == 0) {
            S = epsilon_element;
            prev_epsilon = epsilon_element;
        } else {
            S += epsilon_element;
            V += (epsilon_element != prev_epsilon);
            prev_epsilon = epsilon_element;
        }
    }

    if (k == N) {
        *res_S = S;
        *res_V = V;

        // Reset static variables for the next call
        S = 0;
        V = 1;
        prev_epsilon = 0;
        k = 0;
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
    printf("result S = %d\n", result_V);  // Changed from 'V' to 'S'

    return 0;
}