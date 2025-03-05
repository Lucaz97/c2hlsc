
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
    // For n <= 1, there are no positive odd numbers less than n.
    if (n <= 1)
        return 0;

    // Compute the number of odd terms; note that for any n > 1,
    // the number of odd integers in the range [1, n) is n/2.
    int count = n / 2;
    // The sum of the first 'count' odd numbers is given by count*count.
    // This arithmetic formulation preserves the original functionality while
    // eliminating the loop for optimum latency.
    return count * count;
}

int even_sum(int n)
{
  int sum = 0;
  // Fully unroll the loop to minimize latency
  #pragma hls_unroll yes
  for (int i = 2; i < n; i += 2)
  {
    sum += i;
  }
  return sum;
}

void compute5(int n[5])
{
    // Manually replicate the computations for each index to allow full parallelism
    // and enable the HLS optimizer to inline and schedule operations concurrently.
    int tmp0, tmp1;

    // Computation for element 0
    tmp0 = fibonacci(n[0]);
    tmp1 = even_sum(n[0]);
    n[0] = fibonacci(tmp1) - odd_sum(tmp0);

    // Computation for element 1
    tmp0 = fibonacci(n[1]);
    tmp1 = even_sum(n[1]);
    n[1] = fibonacci(tmp1) - odd_sum(tmp0);

    // Computation for element 2
    tmp0 = fibonacci(n[2]);
    tmp1 = even_sum(n[2]);
    n[2] = fibonacci(tmp1) - odd_sum(tmp0);

    // Computation for element 3
    tmp0 = fibonacci(n[3]);
    tmp1 = even_sum(n[3]);
    n[3] = fibonacci(tmp1) - odd_sum(tmp0);

    // Computation for element 4
    tmp0 = fibonacci(n[4]);
    tmp1 = even_sum(n[4]);
    n[4] = fibonacci(tmp1) - odd_sum(tmp0);
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


