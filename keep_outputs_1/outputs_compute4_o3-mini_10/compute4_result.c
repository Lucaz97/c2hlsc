
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int odd_factorial(int n)
{
    int res = 1;
#pragma hls_unroll yes
    for (int i = 1; i < n; i += 2)
    {
        res = res * i;
    }
    return res;
}

int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  
  // Latency-optimized: Fully unroll the loop to compute all iterations in parallel
  #pragma hls_unroll yes
  for (int i = 2; i < n; i++)
  {
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}


int even_sum(int n)
{
  // Instead of iterating, compute the sum with a closed-form expression.
  // The loop: for (i = 2; i < n; i += 2) sums even numbers: 2 + 4 + 6 + ... + 2*m,
  // where m = floor((n-1)/2). This sum equals 2 * (1 + 2 + ... + m) = m*(m+1).
  if (n <= 2)
    return 0;
  int m = (n - 1) / 2;
  return m * (m + 1);
}

int compute4(int n)
{
  int result0;
  int result1;
  int temp[2];

  // Execute odd_factorial and fibonacci concurrently.
  // Then compute the two successive even_sum operations in a fully unrolled loop
  // so that the dependent operations are merged into one combinational chain,
  // thereby minimizing the latency.
#pragma HLS dataflow
  {
    // Compute odd_factorial in parallel
    #pragma HLS inline
    result0 = odd_factorial(n);
  }
  {
    // Compute fibonacci in parallel
    #pragma HLS inline
    result1 = fibonacci(n);
  }

  // Use a 2-iteration loop and fully unroll it to collapse the chained even_sum calls.
  // Iteration 0 computes: temp[0] = even_sum(result0 - result1)
  // Iteration 1 computes: temp[1] = even_sum(temp[0] - result1)
  for (int i = 0; i < 2; i++) {
#pragma HLS unroll
    if (i == 0) {
      temp[i] = even_sum(result0 - result1);
    } else {
      temp[i] = even_sum(temp[0] - result1);
    }
  }

  return temp[1];
}
int main()
{
  int n = 0x8;
  int ret;
  ret = compute4((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


