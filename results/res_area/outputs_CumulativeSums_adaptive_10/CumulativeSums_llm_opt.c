#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000

void CumulativeSums(int *res_sup, int *res_inf, int current_value, int *sup, int *inf)
{
    static int S = 0;
    static int sup_count = 0;
    static int inf_count = 0;

    // Update S based on the current value
    S += (current_value) ? 1 : -1;

    // Update sup and inf counts
    if (S > sup_count) {
        sup_count++;
    }
    if (S < inf_count) {
        inf_count--;
    }

    // Assign results to output pointers
    *res_sup = sup_count;
    *res_inf = inf_count;
}

int main(){
    extern int epsilon[]; // Use the existing definition of epsilon
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int res_sup;
    int res_inf;
    int sup = 0;
    int inf = 0;
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i], &sup, &inf);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
}
