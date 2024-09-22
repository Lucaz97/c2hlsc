#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#include <stdio.h>

#define N 16
#define BLOCK_SIZE 8

void BlockFrequency(int *is_random, int block[BLOCK_SIZE]) 
{
    int j;
    int blockSum;
    long long sum = 0;          // Use long long for sum to accommodate larger values
    long long pi_scaled;        // Scaled version of pi
    long long v_scaled;         // Scaled version of v
    long long v_squared_sum = 0; // Sum of scaled v squared

    for (int i = 0; i < N; i++) 
    {
        blockSum = 0; // Reset blockSum for each block
        for (j = 0; j < BLOCK_SIZE; j++)
        {
            blockSum += block[j];
        }

        // Calculate pi * 10000
        pi_scaled = (long long)blockSum * 10000 / BLOCK_SIZE; 
        // v = pi - 0.5 -> v_scaled = pi_scaled - 5000
        v_scaled = pi_scaled - 5000; 
        v_squared_sum += (v_scaled * v_scaled) / 10000;  // v^2, scaled down by 10000
    }

    // Threshold: 0.027 * N * 10000 = 270 * N
    *is_random = v_squared_sum < (270 * N);
}

int main() 
{
    int epsilon[N * BLOCK_SIZE];
    int is_random = 1; // Initialize as not random

    for (int i = 0; i < N * BLOCK_SIZE; i++) 
    {
        epsilon[i] = i * 73 % 7 == 0;
    }

    for (int i = 0; i < N; i++)
    {
        BlockFrequency(&is_random, &epsilon[i * BLOCK_SIZE]);
    }

    printf("is_random = %d\n", is_random);
}
