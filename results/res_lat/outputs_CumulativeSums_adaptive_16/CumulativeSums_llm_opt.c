#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_elem)
{
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    // Update S based on the current element
    S += (epsilon_elem) ? 1 : -1;

    // Update sup and inf based on the value of S
    if (S > sup) {
        sup++;
    }
    if (S < inf) {
        inf--;
    }

    // Set the results at the end of processing
    *res_sup = sup;
    *res_inf = inf;
}

int main(){
    extern int epsilon[]; // Use the externally defined epsilon array
    int i;
    int res_sup;
    int res_inf;

    // Initialize epsilon array
    for (i = 0 ; i < 20000; i++){
        epsilon[i] = i*73%7 ==0;
    }

    // Process each element in the epsilon array
    #pragma hls_pipeline_init_interval 1
    for (i = 0; i < 20000; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
}
