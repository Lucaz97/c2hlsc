#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000
// Note: epsilon array is already defined externally

void CumulativeSums(int *res_sup, int *res_inf) {
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    #pragma hls_pipeline_init_interval 1
    for (int i = 0; i < N; i++) {
        #pragma hls_unroll 4
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
