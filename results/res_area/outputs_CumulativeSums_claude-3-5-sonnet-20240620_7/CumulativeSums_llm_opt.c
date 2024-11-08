#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000

void CumulativeSums(int *res_sup, int *res_inf) {
    #pragma HLS INTERFACE ap_none port=res_sup
    #pragma HLS INTERFACE ap_none port=res_inf
    #pragma HLS INTERFACE ap_ctrl_none port=return

    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    #pragma HLS RESET variable=S
    #pragma HLS RESET variable=sup
    #pragma HLS RESET variable=inf

    #pragma HLS PIPELINE off

    for (int i = 0; i < N; i++) {
        #pragma HLS LOOP_TRIPCOUNT min=20000 max=20000
        if (epsilon[i]) {
            S++;
        } else {
            S--;
        }

        if (S > sup) {
            sup++;
        }
        if (S < inf) {
            inf--;
        }
    }

    *res_sup = sup;
    *res_inf = inf;

    // Reset static variables for next use
    S = 0;
    sup = 0;
    inf = 0;
}

int main() {
    for (int i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }

    int res_sup;
    int res_inf;
    CumulativeSums(&res_sup, &res_inf);

    printf("sup = %d - inf = %d\n", res_sup, res_inf);

    return 0;
}
