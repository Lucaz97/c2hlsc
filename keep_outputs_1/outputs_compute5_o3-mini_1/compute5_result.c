
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  #pragma hls_unroll yes
  for (int i = 2; i < n; i++)
  {
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int odd_sum(int n)
{
    int sum = 0;
    #pragma hls_pipeline_init_interval 1
    for (int i = 1; i < n; i += 2)
    {
        sum += i;
    }
    return sum;
}

int even_sum(int n)
{
  // For n less than 3, the loop would not execute
  if (n < 3) return 0;

  // Compute the number of even terms (i.e., iterations)
  int count = (n - 1) / 2; // floor division: works for n >= 3

  // Use closed-form summation: sum = 2 + 4 + ... + (2*count)
  // This simplifies to: sum = count * (count + 1)
  return count * (count + 1);
}

void compute5(int n[5])
{
    int result0;
    int result1;
    int result2;
    int result3;
    
    // Fully unroll the loop to achieve minimal latency.
    // This replicates the computation for each of the 5 elements concurrently.
#pragma HLS unroll yes
    for (int i = 0; i < 5; i++)
    {
        result0 = fibonacci(n[i]);
        result1 = odd_sum(result0);
        result2 = even_sum(n[i]);
        result3 = fibonacci(result2);
        n[i] = result3 - result1;
    }
}
int main()
{
  unsigned int n[] = {6, 7, 8, 9, 10};
  compute5((int *) n);
  for (int _i = 0; _i < 5; _i++)
  {
    printf("%d ", n[_i]);
  }

  printf("\n");
}


