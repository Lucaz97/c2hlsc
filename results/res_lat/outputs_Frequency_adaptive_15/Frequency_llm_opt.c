#include <stdio.h>

#define N 128
int epsilon[N]; // array of 0s and 1s

#define N 128

void Frequency(int *result, int x)
{
    static int sum = 0;
    static int count = 0;

    sum += (2 * ((int) x)) - 1;

    // To return the result after processing all elements
    count++;
    if (count == N) {
        *result = sum;
        count = 0; // Reset for the next batch of inputs
        sum = 0;   // Reset sum for the next calculation
    }
}

int main(){
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int result = 0;

    // Call Frequency for each element in epsilon
    for (i = 0; i < N; i++) {
        Frequency(&result, epsilon[i]);
    }

    printf("result = %d\n", result);
}
