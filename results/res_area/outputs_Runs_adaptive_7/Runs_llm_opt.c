#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535
extern int epsilon[N]; // Assume epsilon is defined elsewhere

void Runs(int *res_S, int *res_V, int x, int *first_call) {
    static int S = 0;
    static int V = 1;
    static int prev_value = -1; // Initialize to an invalid state

    if (x) {
        S++;
    }

    if (*first_call) {
        *first_call = 0; // Mark that the first call has been made
        prev_value = x; // Set the previous value for the first call
    } else {
        if (x != prev_value) {
            V++;
        }
    }

    prev_value = x; // Update the previous value for the next call

    *res_S = S;
    *res_V = V;
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S = 0, result_V = 0;
    int first_call = 1;
    for (i = 0; i < N; i++) {
        Runs(&result_S, &result_V, epsilon[i], &first_call);
    }

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V); // Corrected to match expected output
    return 0;
}
