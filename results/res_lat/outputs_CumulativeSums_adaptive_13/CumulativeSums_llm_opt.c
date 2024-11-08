#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000
extern int epsilon[]; // Use the externally defined epsilon array

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
        sup = S;
    }
    if (S < inf) {
        inf = S;
    }

    // Assign results to output pointers
    *res_sup = sup;
    *res_inf = inf;
}

int main() {
    int i;
    int res_sup;
    int res_inf;

    // Initialize the epsilon array
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }

    // Pipeline the loop to optimize for latency
    #pragma hls_pipeline_init_interval 1
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
    return 0;
}
