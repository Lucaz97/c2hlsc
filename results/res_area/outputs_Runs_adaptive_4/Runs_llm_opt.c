#include <stdio.h>

#define N 65535
int epsilon[N]; // array of 0s and 1s


#define N 65535
extern int epsilon[N]; // Declare epsilon as an external array

void Runs(int *res_S, int *res_V, int x, int last_call) {
    static int S = 0;
    static int V = 1;
    static int prev_value = -1; // Initialize to an invalid state
    static int initialized = 0; // To track if the first call has been made

    if (x) {
        S++;
    }

    if (initialized) {
        if (x != prev_value) {
            V++;
        }
    } else {
        initialized = 1; // Mark that the first call has been made
    }

    prev_value = x;

    // Only set results on the last call
    if (last_call) {
        *res_S = S;
        *res_V = V;
    }
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result_S = 0, result_V = 0;
    for (i = 0; i < N; i++) {
        Runs(&result_S, &result_V, epsilon[i], i == N - 1);
    }

    printf("result S = %d\n", result_S);
    printf("result S = %d\n", result_V); // Change "result V" to "result S"

    return 0;
}
