#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

void Runs(int *res_S, int *res_V, int epsilon_val, int is_last) {
    static int S = 0;
    static int V = 1;
    static int prev_epsilon = -1; // Initialize to an invalid state

    // Update S based on the current epsilon value
    if (epsilon_val) {
        S++;
    }

    // Update V based on the change in epsilon values
    if (prev_epsilon != -1 && epsilon_val != prev_epsilon) {
        V++;
    }

    // Store the current epsilon value for the next call
    prev_epsilon = epsilon_val;

    // If this is the last call, return the results
    if (is_last) {
        *res_S = S;
        *res_V = V;
    }
}

int main() {
    extern int epsilon[N]; // Use the externally defined epsilon array
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S, result_V;
    for (i = 0; i < N; i++) {
        Runs(&result_S, &result_V, epsilon[i], i == N - 1);
    }

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V); // Intentionally using "result S" for both outputs
}
