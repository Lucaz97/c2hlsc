#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_val)
{
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    // Update S based on the current epsilon value
    S += (epsilon_val) ? 1 : -1;

    // Update sup to the maximum value of S
    if (S > sup) {
        sup = S;
    }

    // Update inf to the minimum value of S
    if (S < inf) {
        inf = S;
    }

    // Assign the results to the output pointers
    *res_sup = sup;
    *res_inf = inf;
}

int main(){
    extern int epsilon[]; // Use the externally defined epsilon array
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int res_sup;
    int res_inf;

    #pragma hls_pipeline_init_interval 1
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
}
