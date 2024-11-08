#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535

extern int epsilon[N]; // Assume epsilon is defined elsewhere

void Runs(int *res_S, int *res_V) {
    int state_S = 0;
    int state_V = 0;
    int last_input = epsilon[0]; // Initialize to the first element

    for (int i = 0; i < N; i++) {
        int input = epsilon[i];

        // Update the state for S
        if (input) {
            state_S++;
        }

        // Update the state for V
        if (i > 0 && input != last_input) {
            state_V++;
        }
        last_input = input;
    }

    *res_S = state_S;
    *res_V = state_V + 1; // Adjust to account for the initial transition
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S, result_V;
    Runs(&result_S, &result_V);

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V); // Intentionally label this as result S
}
