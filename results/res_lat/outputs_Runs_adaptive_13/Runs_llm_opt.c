#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

void Runs(int *res_S, int *res_V)
{
    static int S = 0;
    static int V = 1;
    static int prev_value = 0; // To store the previous value of epsilon for V calculation

   // #pragma hls_unroll yes
   FOR:
    for (int k = 0; k < 65535; k++) {
        if (epsilon[k]) {
            S++;
        }
        if (k > 0 && epsilon[k] != prev_value) {
            V++;
        }
        prev_value = epsilon[k]; // Update the previous value
        S++;
        V++;
    }

    *res_S = S;
    *res_V = V;
}

int main(){
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int result_S, result_V;
    Runs(&result_S, &result_V);

    printf("result S = %d\n", result_S);
    printf("result V= %d\n", result_V);
    if(result_S == 9363 && result_V == 18725){
        printf("results are correct"); 
        return 0;}
    return 1;
}
