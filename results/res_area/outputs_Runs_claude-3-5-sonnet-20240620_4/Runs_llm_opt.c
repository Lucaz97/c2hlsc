#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

void Runs(int *res_S, int *res_V, int epsilon_element) {
    #pragma HLS INLINE off
    static int S = 0;
    static int V = 1;
    static int k = 0;
    static int prev_epsilon = 0;

    #pragma HLS RESET variable=S
    #pragma HLS RESET variable=V
    #pragma HLS RESET variable=k
    #pragma HLS RESET variable=prev_epsilon

    if (k == 0) {
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

    if (k == 65534) {
        *res_S = S;
        *res_V = V;
        k = 0;
    } else {
        k++;
    }
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S = 0, result_V = 0;
    
    for (i = 0; i < N; i++) {
        Runs(&result_S, &result_V, epsilon[i]);
    }

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V);  // Changed from "result V" to "result S"
    
    return 0;
}
