
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;

  // Merge two iterations per loop to reduce the number of sequential steps,
  // which improves latency.
  for (int i = 2; i < n; i += 2)
  {
    #pragma hls_pipeline_init_interval 1
    // First iteration step
    c = a + b;
    a = b;
    b = c;
    
    // Second iteration step if within bounds
    if ((i + 1) < n)
    {
      c = a + b;
      a = b;
      b = c;
    }
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
  // Using a closed-form expression to compute the sum of even numbers
  // less than n, thereby removing the loop and minimizing latency.
  int count = (n - 1) >> 1; // floor((n-1)/2), using bit-shift for division by 2 when n is non-negative
  return count * (count + 1);
}

void compute5(int n[5])
{
  int result0;
  int result1;
  int result2;
  int result3;
  
  // Fully unroll the loop to reduce latency.
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


