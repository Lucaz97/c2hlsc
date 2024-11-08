#include <stdio.h>

#define N 128
int epsilon[N]; // array of 0s and 1s

#define N 128

void Frequency(int *result, int x)
{
    static int sum = 0;
    sum += (2 * ((int) x)) - 1;

    // To output the result after processing all elements
    static int count = 0;
    count++;
    if (count == N) {
        *result = sum;
        count = 0; // Reset count for the next series of inputs
        sum = 0;   // Reset sum for the next series of inputs
    }
}

int main(){
    int i;
    int epsilon[N]; // Declare epsilon array locally in main
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int result;
    for (i = 0; i < N; i++) {
        Frequency(&result, epsilon[i]);
    }

    printf("result = %d\n", result);
}
