#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#define N 16
#define M 8

void BlockFrequency(int *result, int blockIndex, int blockSum)
{
    long long sum = 0;
    long long pi;
    long long v;
    
    // Calculate pi and v for the given block using integer arithmetic
    pi = (blockSum * 1000000) / 8; // Scale by 1000000 to maintain precision
    v = pi - 500000; // Equivalent to pi - 0.5 in scaled form
    sum += (v * v) / 1000000; // Scale back the result

    // Accumulate the result for the specific block index
    static long long totalSum[16] = {0}; // static to preserve across function calls
    totalSum[blockIndex] += sum;

    // If we have processed all blocks, compute the final result
    if (blockIndex == 15) {
        *result = 0;
        for (int i = 0; i < 16; i++) {
            *result += totalSum[i];
        }
    }
}

int main(){
    int epsilon[N*M]; // array of 0s and 1s
    int i;
    for (i = 0 ; i < N*M; i++){
        epsilon[i] = i*73%7 ==0;
    }
    int result;
    int blockSum = 0;
    for (int blockIndex = 0; blockIndex < 16; blockIndex++) {
        blockSum = 0;
        for (int j = 0; j < 8; j++) {
            blockSum += epsilon[blockIndex * 8 + j];
        }
        BlockFrequency(&result, blockIndex, blockSum);
    }

    // Convert the result back to floating-point for display
    printf("result = %f\n", result / 1000000.0);
    return 0;
}
