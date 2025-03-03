
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  // Group two odd multiplications per iteration to shorten the dependency chain.
  #pragma hls_pipeline_init_interval 1
  for (int i = 1; i < n; i += 4)
  {
    int prod = i;
    if ((i + 2) < n)
      prod = prod * (i + 2);
    res = res * prod;
  }
  return res;
}


int even_sum(int n)
{
  // Use dual accumulators to break the dependency chain,
  // which reduces the critical path delay (i.e. latency)
  // while keeping the area low.
  int sum0 = 0, sum1 = 0;

  // Process two even numbers per iteration
  for (int i = 2; i < n; i += 4)
  {
    #pragma hls_pipeline_init_interval 0
    sum0 += i;
    if (i + 2 < n)
      sum1 += (i + 2);
  }

  return sum0 + sum1;
}

int compute2(int n)
{
  int result1;
  int result2;
  #pragma HLS dataflow
  result1 = odd_factorial(n);
  result2 = even_sum(n);
  return result1 - result2;
}
int main()
{
  int n = 0xb;
  int ret;
  ret = compute2((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


