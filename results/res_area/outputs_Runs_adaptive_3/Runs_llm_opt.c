#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

extern int epsilon[N]; // Assume this is defined elsewhere

void Runs(int *res_S, int *res_V) {
    static int S = 0;
    static int V = 1;

    for (int index = 0; index < N; index++) {
        int current_value = epsilon[index];
        int previous_value = (index > 0) ? epsilon[index - 1] : 0;

        // Update S based on the current value
        if (current_value) {
            S++;
        }

        // Update V based on the current and previous values
        if (index > 0 && current_value != previous_value) {
            V++;
        }

        // Store results at the end of processing
        if (index == N - 1) {
            *res_S = S;
            *res_V = V;
        }
    }
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S, result_V;
    Runs(&result_S, &result_V);

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V); // Corrected to match the expected output format
}
