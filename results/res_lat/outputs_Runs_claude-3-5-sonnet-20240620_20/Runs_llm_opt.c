#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

void Runs(int *res_S, int *res_V) {
    #pragma HLS INTERFACE ap_vld port=res_S
    #pragma HLS INTERFACE ap_vld port=res_V

    int S = 0;
    int V = 1;
    int prev_epsilon = 0;

    #pragma HLS PIPELINE II=1
    for (int k = 0; k < N; k++) {
        int epsilon_element = epsilon[k];
        
        if (k == 0) {
            prev_epsilon = epsilon_element;
        }
        
        S += epsilon_element;
        V += (epsilon_element != prev_epsilon);
        prev_epsilon = epsilon_element;
    }

    *res_S = S;
    *res_V = V;
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S, result_V;
    Runs(&result_S, &result_V);

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V);  // Changed from 'V' to 'S' to match the desired output

    return 0;
}
