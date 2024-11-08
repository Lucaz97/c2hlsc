#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

extern int epsilon[N]; // Assume epsilon is defined elsewhere

void Runs(int *res_S, int *res_V, int index, int *S, int *V)
{
    #pragma HLS pipeline II=1
    if (index == 0) {
        *S = 0;
        *V = 1;
    }

    if (index < N) {
        if (epsilon[index]) {
            (*S)++;
        }
        if (index > 0 && epsilon[index] != epsilon[index - 1]) {
            (*V)++;
        }
    }

    if (index == N - 1) {
        *res_S = *S;
        *res_V = *V;
    }
}

int main(){
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int result_S = 0, result_V = 0;
    int S = 0, V = 0;
    for (i = 0; i < N; i++) {
        Runs(&result_S, &result_V, i, &S, &V);
    }

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V);
}
