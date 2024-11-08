#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_val)
{
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    // Update S based on the current epsilon value
    S += (epsilon_val) ? 1 : -1;

    // Update sup and inf based on the value of S
    if (S > sup) {
        sup++;
    }
    if (S < inf) {
        inf--;
    }

    // Set the results when the last element is processed
    // This can be handled externally to determine when to read the results
    *res_sup = sup;
    *res_inf = inf;
}

int main(){
    extern int epsilon[]; // Use the existing definition of epsilon
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int res_sup;
    int res_inf;
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
}
