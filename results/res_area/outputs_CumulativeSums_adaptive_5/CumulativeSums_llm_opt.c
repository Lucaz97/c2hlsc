#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

void CumulativeSums(int *res_sup, int *res_inf)
{
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    for (int index = 0; index < N; index++) {
        (epsilon[index]) ? (S++) : (S--);
        if (S > sup)
            sup++;
        if (S < inf)
            inf--;
    }

    *res_sup = sup;
    *res_inf = inf;
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
