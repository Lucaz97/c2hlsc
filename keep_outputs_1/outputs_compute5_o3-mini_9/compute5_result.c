
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c = 0;

  // Optimize for latency by unrolling the loop fully
  for (int i = 2; i < n; i++)
  {
    #pragma hls_unroll yes
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

// We factor out the multiplication into an inline helper and force
// its implementation to use a DSP block. This closed‐form computation
// offers a latency of 1 cycle while keeping area low if the multiplication
// is mapped to dedicated DSP resources.
static inline int mul(int a, int b)
{
  #pragma HLS inline
  #pragma HLS RESOURCE core=DSP48  // Map the multiplication to a DSP48 block.
  return a * b;
}

int odd_sum(int n)
{
  // Note: For any n, floor(n/2)^2 equals the sum of all odd numbers from 1 to (n-1).
  // We use a bit‐wise shift for division by 2.
  int k = n >> 1;
  return mul(k, k);
}

int even_sum(int n)
{
    // Compute the count of even numbers below n without using branches.
    // For n even: count = (n - 2) / 2.
    // For n odd:  count = (n - 1 - 1) / 2 = (n - 2 + 1) / 2 = (n - 1) / 2.
    // The unified expression is: count = ((n - 2) + (n & 1)) >> 1.
    int count = ((n - 2) + (n & 1)) >> 1;
    return count * (count + 1);
}

void compute5(int n[5])
{
  // Pipeline the loop to reduce overall latency while avoiding full unrolling to keep area low.
  #pragma hls_pipeline_init_interval 1
  for (int i = 0; i < 5; i++)
  {
    int result0 = fibonacci(n[i]);
    int result1 = odd_sum(result0);
    int result2 = even_sum(n[i]);
    int result3 = fibonacci(result2);
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


