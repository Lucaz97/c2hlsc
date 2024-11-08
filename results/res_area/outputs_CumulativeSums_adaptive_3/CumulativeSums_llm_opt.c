#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000

void CumulativeSums(int *res_sup, int *res_inf, int index)
{
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    (epsilon[index]) ? (S++) : (S--);
    if (S > sup)
        sup++;
    if (S < inf)
        inf--;

    *res_sup = sup;
    *res_inf = inf;
}

int main(){
    int i;
    // Assuming epsilon is defined elsewhere
    extern int epsilon[N]; // Use extern to reference the existing array

    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int res_sup = 0;
    int res_inf = 0;

    // Call CumulativeSums for each index in the epsilon array
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, i);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
    return 0;
}
