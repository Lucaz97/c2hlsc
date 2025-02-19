#include <stdio.h>

#define N 20000
int epsilon[N]; // array of 0s and 1s

#define N 20000
extern int epsilon[]; // Use the externally defined epsilon array

void CumulativeSums(int *res_sup, int *res_inf, int epsilon_val)
{
    static int S = 0;
    static int sup = 0;
    static int inf = 0;

    // Update S based on the incoming epsilon value
    S += (epsilon_val) ? 1 : -1;

    // Update sup and inf based on the current value of S
    if (S > sup) {
        sup = S;
    }
    if (S < inf) {
        inf = S;
    }

    // Set the results only when the last element is processed
    *res_sup = sup;
    *res_inf = inf;
}

int main(){
    int i;
    int res_sup = 0;
    int res_inf = 0;

    // Initialize the epsilon array
    for (i = 0; i < N; i++) {
        epsilon[i] = (i * 73 % 7 == 0);
    }

    // Process each element in the epsilon array
    for (i = 0; i < N; i++) {
        CumulativeSums(&res_sup, &res_inf, epsilon[i]);
    }

    printf("sup = %d - inf = %d\n", res_sup, res_inf);
}
