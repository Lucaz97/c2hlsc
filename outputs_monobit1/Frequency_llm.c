#include <stdio.h>

#define N 128
int epsilon[N]; // array of 0s and 1s
#define N 128

void Frequency(int *is_random, int x) {
    static int sum = 0; // Change sum to int to avoid floating-point operations
    static int count = 0;

    sum += (2 * x) - 1;
    count++;

    if (count == N) {
        *is_random = (sum >= -29) && (sum <= 29);
        sum = 0; // Reset sum for the next full calculation
        count = 0; // Reset count for the next full calculation
    }
}

int main() {
    int i;
    int epsilon[N]; // Move the definition of epsilon here
    for (i = 0; i < N; i++) {
        epsilon[i] = (i * 73 % 7 == 0);
    }
    
    int is_random = 0; // Initialize is_random to avoid uninitialized usage
    for (i = 0; i < N; i++) {
        Frequency(&is_random, (int)epsilon[i]); // Explicitly cast the value to int
    }

    printf("is_random = %d\n", is_random);
}
