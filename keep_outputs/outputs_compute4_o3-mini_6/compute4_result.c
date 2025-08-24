
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int odd_factorial(int n)
{
    // Two-accumulator approach: split the odd factors into two groups
    // to shorten the multiplication dependency chain.
    int prod_even_index = 1;
    int prod_odd_index  = 1;
    
    // Determine how many factors (odd numbers) are to be multiplied.
    // Original loop runs for odd numbers i in [1, n) i.e. i = 1,3,5,... < n.
    // The number of iterations = n/2 (integer division).
    int num_factors = n / 2;

    for (int j = 0; j < num_factors; j++)
    {
    #pragma hls_unroll yes
        int odd = 2 * j + 1;
        // Alternate accumulation between the two registers.
        if (j & 1)
            prod_odd_index = prod_odd_index * odd;
        else
            prod_even_index = prod_even_index * odd;
    }
    
    // Combine the two partial products.
    return prod_even_index * prod_odd_index;
}

int fibonacci(int n)
{
  // Handle small n as in the original function.
  if (n < 2)
    return n;
  
  // The original fibonacci() returns F(n-1). Compute m = n - 1.
  int m = n - 1;
  
  // Compute the number of bits in m.
  int shift = 0;
  int tmp = m;
  while(tmp)
  {
    #pragma hls_pipeline_init_interval 0
    tmp >>= 1;
    shift++;
  }
  shift--;  // Adjust because the loop overshoots by one
  
  // Initialize to F(0) and F(1)
  int a = 0; // F(0)
  int b = 1; // F(1)
  
  // Fast doubling iteration: process each bit of m from most significant to least significant.
  for (int i = shift; i >= 0; i--)
  {
    #pragma hls_pipeline_init_interval 0
    int c = a * ((b << 1) - a); // c = F(2k) = F(k) * [2*F(k+1) – F(k)]
    int d = a * a + b * b;      // d = F(2k+1) = F(k)^2 + F(k+1)^2
    
    if (m & (1 << i))
    {
      a = d;
      b = c + d;
    }
    else
    {
      a = c;
      b = d;
    }
  }
  
  // a now holds F(n-1). For example, fibonacci(8) returns 13 and fibonacci(7) returns 8.
  return a;
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


