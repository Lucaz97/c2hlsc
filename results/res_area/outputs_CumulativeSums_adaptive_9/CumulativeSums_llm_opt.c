#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_val)
{
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    S += (epsilon_val) ? 1 : -1;
    
    if (S > sup) {
        sup++;
    }
    if (S < inf) {
        inf--;
    }

    *res_sup = sup;
    *res_inf = inf;
}

int main(){
    extern int epsilon[]; // Use the existing definition of epsilon
    int i;
    for (i = 0 ; i < 20000; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int res_sup = 0;
    int res_inf = 0;

    for (i = 0; i < 20000; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
}
