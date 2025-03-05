
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int odd_factorial(int n)
{
  int res = 1;
  for (int i = 1; i < n; i += 2) {
    #pragma hls_unroll yes
    res = res * i;
  }
  return res;
}

int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c = 0;
  
  // Fully unroll the loop to create a pure combinational chain.
  // Also force a pipeline initiation interval of 0 so that each unrolled operation
  // can be registered to minimize the latency.
  for (int i = 2; i < n; i++)
  {
    #pragma hls_pipeline_init_interval 0
    #pragma hls_unroll yes
    c = a + b;
    a = b;
    b = c;
  }
  
  return c;
}

int even_sum(int n)
{
  int sum = 0;
  // The loop is fully unrolled to minimize latency.
  for (int i = 2; i < n; i += 2)
  {
    #pragma hls_unroll yes
    sum += i;
  }

  return sum;
}

int compute4(int n)
{
    #pragma HLS inline

    int fib = fibonacci(n);
    // Fully inline odd_factorial and even_sum calls so that the operations are merged together.
    return even_sum( even_sum( odd_factorial(n) - fib ) - fib );
}
int main()
{
  int n = 0x8;
  int ret;
  ret = compute4((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


