#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

void Runs(int *res_S, int *res_V) {
    static int S = 0;
    static int V = 1;
    static int previous_value = -1; // Initialize to an invalid state

    // Iterate over the epsilon array
    for (int current_index = 0; current_index < N; current_index++) {
        // Update S based on the current element
        if (epsilon[current_index]) {
            S++;
        }

        // Update V based on the current and previous elements
        if (current_index > 0 && epsilon[current_index] != previous_value) {
            V++;
        }

        // Store the current value for the next call
        previous_value = epsilon[current_index];
    }

    // Output the results
    *res_S = S;
    *res_V = V;
}

int main() {
    extern int epsilon[N]; // Declare the external array
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S, result_V;
    Runs(&result_S, &result_V);

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V); // Corrected to match expected output
}
