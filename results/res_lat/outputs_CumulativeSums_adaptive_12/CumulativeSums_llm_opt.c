#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000

void CumulativeSums(int *res_sup, int *res_inf)
{
    static int S = 0;
    static int sup = 0;
    static int inf = 0;
    static int count = 0;

    // Apply pipelining to the function to improve latency
    #pragma hls_pipeline_init_interval 1
    for (int i = 0; i < N; i++) {
        // Update S based on the current epsilon value
        S += (epsilon[i]) ? 1 : -1;

        // Update sup and inf based on the value of S
        if (S > sup) {
            sup++;
        }
        if (S < inf) {
            inf--;
        }

        // Assign results when the last element is processed
        count++;
        if (count == N) {
            *res_sup = sup;
            *res_inf = inf;
            count = 0; // Reset count for the next series of calls
        }
    }
}

int main(){
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int res_sup;
    int res_inf;
    CumulativeSums(&res_sup, &res_inf);

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
}
