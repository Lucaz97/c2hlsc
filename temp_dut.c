
#include <stdio.h>

#define N 20000

// Function to perform cumulative sums in a streaming manner
void CumulativeSums(int *is_random, int epsilon) {
    static int S = 0, sup = 0, inf = 0;
    
    S += (epsilon ? 1 : -1);

    if (S > sup) {
        sup++;
    }
    if (S < inf) {
        inf--;
    }

    // After processing all elements, determine if the series is random
    static int count = 0;
    count++;

    if (count == N) {
        if (sup < 397 && inf > -397) {
            *is_random = 1;
        } else {
            *is_random = 0;
        }
        count = 0; // Reset the counter for the next use
        S = 0;    // Reset S for fresh calculations
        sup = 0;  // Reset sup for fresh calculations
        inf = 0;  // Reset inf for fresh calculations
    }
}

int main() {
    int i;
    int is_random = 0;

    // Populate the epsilon array and call CumulativeSums for each element
    for (i = 0; i < N; i++) {
        int epsilon = (i * 73 % 7) == 0;
        CumulativeSums(&is_random, epsilon);
    }

    printf("is_random = %d\n", is_random);

    return 0;
}
