#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_val);

int main() {
    int epsilon[N]; // array of 0s and 1s
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int res_sup = 0;
    int res_inf = 0;
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
    return 0;
}

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_val) {
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    S += (epsilon_val) ? 1 : -1;
    
    if (S > sup) {
        sup = S;
    }
    if (S < inf) {
        inf = S;
    }

    *res_sup = sup;
    *res_inf = inf;
}
