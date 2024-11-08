#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

void Runs(int *res_S, int *res_V)
{
    static int S = 0;
    static int V = 1;
    static int prev_value = -1; // Initialize to an invalid state

    for (int index = 0; index < N; index++) {
        // Update S based on the current index
        if (epsilon[index]) {
            S++;
        }

        // Update V based on the current index
        if (index > 0 && epsilon[index] != prev_value) {
            V++;
        }

        // Store the current value for the next call
        prev_value = epsilon[index];
    }

    // Output results when the last index is reached
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
    printf("result S = %d\n", result_V); // Intentionally using "result S" for both outputs
}
