
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;

  // Full loop unrolling to minimize the loop control overhead
  // and maximize latency reduction.
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
  // Fully unroll the loop to reduce latency
  for (int i = 1; i < n; i += 2)
  {
    #pragma hls_unroll yes
    sum += i;
  }
  return sum;
}

int even_sum(int n)
{
  int sum = 0;
  // Pipeline the fully-unrolled reduction tree to achieve a combinational
  // adder tree with a one-cycle latency.
  #pragma hls_pipeline_init_interval 1
  for (int i = 2; i < n; i += 2)
  {
    #pragma hls_unroll yes
    sum += i;
  }
  return sum;
}

int compute3(int n)
{
  // Enable a dataflow region so that the two independent branches execute concurrently,
  // thereby reducing the overall latency.
#pragma HLS dataflow

  int result0;
  int result1;
  int result2;
  int result3;

  // Branch 1: Compute fibonacci(n) then odd_sum(result0)
  result0 = fibonacci(n);
  result1 = odd_sum(result0);

  // Branch 2: Compute even_sum(n) then fibonacci(result2)
  result2 = even_sum(n);
  result3 = fibonacci(result2);

  return result3 - result1;
}
int main()
{
  int n = 0xb;
  int ret;
  ret = compute3((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


