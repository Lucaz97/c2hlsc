#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_element) {
    #pragma HLS INLINE off
    static int S = 0;
    static int sup = 0;
    static int inf = 0;
    static int k = 0;

    #pragma HLS RESET variable=S
    #pragma HLS RESET variable=sup
    #pragma HLS RESET variable=inf
    #pragma HLS RESET variable=k

    if (k < N) {
        #pragma HLS PIPELINE off
        (epsilon_element) ? (S++) : (S--);
        if (S > sup)
            sup = S;
        if (S < inf)
            inf = S;
        k++;
    }

    if (k == N) {
        *res_sup = sup;
        *res_inf = inf;
        S = 0;
        sup = 0;
        inf = 0;
        k = 0;
    }
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    
    int res_sup = 0;
    int res_inf = 0;
    
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
    return 0;
}
