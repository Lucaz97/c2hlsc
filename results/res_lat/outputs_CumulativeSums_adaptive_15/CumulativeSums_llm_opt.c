#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_val) {
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    // Update S based on the incoming epsilon value
    if (epsilon_val) {
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
    *res_sup = sup;
    *res_inf = inf;
}

int main() {
    int i;
    // Assuming epsilon is defined elsewhere
    extern int epsilon[N]; // Use the existing definition of epsilon

    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int res_sup;
    int res_inf;

    // Optimize the loop for latency
    #pragma hls_pipeline_init_interval 1
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
}
