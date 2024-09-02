
#include <stdio.h>

#define N 20000

// Global array of 0s and 1s
int epsilon[N]; 

// Streaming function
void CumulativeSums(int *is_random, int input) {
    static int S = 0;
    static int sup = 0;
    static int inf = 0;
    
    if (input) {
        S++;
    } else {
        S--;
    }
    
    if (S > sup) {
        sup++;
    }
    
    if (S < inf) {
        inf--;
    }

    // Set is_random to 1 if within bounds, otherwise set to 0
    if (sup < 397 && inf > -397) {
        *is_random = 1;
    } else {
        *is_random = 0;
    }
}

// Main function to test the CumulativeSums function
int main() {
    // Initialize the epsilon array
    for (int i = 0; i < N; i++) {
        epsilon[i] = (i * 73 % 7 == 0) ? 1 : 0;  // using 1s and 0s
    }

    int is_random = 0;

    // Call CumulativeSums for each element in epsilon
    for (int i = 0; i < N; i++) {
        CumulativeSums(&is_random, epsilon[i]);
    }

    // Print the final result
    printf("is_random = %d\n", is_random);
    return 0;
}
