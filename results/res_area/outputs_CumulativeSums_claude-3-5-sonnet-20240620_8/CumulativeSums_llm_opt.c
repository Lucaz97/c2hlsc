#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000
// Note: epsilon array is assumed to be defined externally

void CumulativeSums(int *res_sup, int *res_inf) {
    #pragma HLS INTERFACE ap_none port=res_sup
    #pragma HLS INTERFACE ap_none port=res_inf
    #pragma HLS INTERFACE ap_ctrl_none port=return

    int S = 0;
    int sup = 0;
    int inf = 0;

    #pragma HLS PIPELINE II=1
    for (int k = 0; k < N; k++) {
        int epsilon_element = epsilon[k];
        S += (epsilon_element) ? 1 : -1;
        sup = (S > sup) ? S : sup;
        inf = (S < inf) ? S : inf;
    }

    *res_sup = sup;
    *res_inf = inf;
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int res_sup;
    int res_inf;
    
    CumulativeSums(&res_sup, &res_inf);

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
    return 0;
}
