
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  for (int i = 2; i < n; i++)
  {
    #pragma hls_pipeline_init_interval 1
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int odd_sum(int n)
{
  // Replace the loop with a closed-form calculation.
  // The original loop sums the odd numbers less than n.
  // The count of odd numbers is n/2 (using integer division),
  // and the sum of the first k odd numbers equals k*k.
  int k = n / 2;
  return k * k;
}

int even_sum(int n)
{
  int sum = 0;
  #pragma hls_unroll yes
  for (int i = 2; i < n; i += 2)
  {
    sum += i;
  }
  return sum;
}

void compute5(int n[5])
{
  // Fully unroll the loop since the iterations are independent.
  // This yields the minimum latency while keeping area at the best level seen so far.
  #pragma HLS unroll yes
  for (int i = 0; i < 5; i++) {
    // Compute intermediate values in a single expression to enable better resource sharing.
    int fib_n   = fibonacci(n[i]);
    int even_val = even_sum(n[i]);
    n[i] = fibonacci(even_val) - odd_sum(fib_n);
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


