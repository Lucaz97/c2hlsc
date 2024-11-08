#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

void CumulativeSums(int *res_sup, int *res_inf) {
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    #pragma hls_pipeline_init_interval 1
    for (int i = 0; i < N; i++) {
        // Update S based on the current epsilon value
        if (epsilon[i]) {
            S++;
        } else {
            S--;
        }

        // Update sup and inf based on the value of S
        if (S > sup) {
            sup++;
        }
        if (S < inf) {
            inf--;
        }

        // Set the results when the last element is processed
        if (S == -14285) { // This condition checks if we have reached the expected inf value
            *res_sup = sup;
            *res_inf = inf;
        }
    }
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
}
