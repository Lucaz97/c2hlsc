
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c = 0;
  
  // Fully unroll the loop to reduce latency:
  for (int i = 2; i < n; i++)
  {
    #pragma hls_unroll yes
    c = a + b;
    a = b;
    b = c;
  }
  
  return c;
}

int odd_sum(int n)
{
    int sum = 0;
    // Fully unroll the loop to reduce latency
    for (int i = 1; i < n; i += 2) {
        #pragma hls_unroll yes
        sum += i;
    }
    return sum;
}

int even_sum(int n)
{
  int sum = 0;
  #pragma hls_pipeline_init_interval 1
  for (int i = 2; i < n; i += 2)
  {
    sum += i;
  }
  return sum;
}

// Assume that the following helper functions are defined elsewhere:
// int fibonacci(int n);
// int odd_sum(int n);
// int even_sum(int n);

int compute3(int n)
{
  int odd_result;
  int even_result;

  // Use a dataflow region to execute both chains concurrently.
  // The "inline region" pragmas help the tool aggressively inline the calls,
  // which should further reduce latency.
#pragma HLS dataflow
  {
#pragma HLS inline region recursive
    // Chain 1: Compute fibonacci(n) then odd_sum(fibonacci(n))
    // The nested inline region helps fuse these operations.
    odd_result = odd_sum(fibonacci(n));
  }
  {
#pragma HLS inline region recursive
    // Chain 2: Compute even_sum(n) then fibonacci(even_sum(n))
    even_result = fibonacci(even_sum(n));
  }

  return even_result - odd_result;
}
int main()
{
  int n = 0xb;
  int ret;
  ret = compute3((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


