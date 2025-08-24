
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  for (int i = 1; i < n; i += 2)
  {
    #pragma hls_unroll yes
    res = res * i;
  }
  return res;
}

int even_sum(int n)
{
  // Replace division by 2 with a right shift to reduce resource usage.
  // Using the closed‐form formula: sum of evens below n = m*(m+1),
  // where m = floor((n-1)/2) = (n-1)>>1.
  int m = (n - 1) >> 1;
  int sum = m * (m + 1);
  return sum;
}

int compute1(int n)
{
  // Use inlining to eliminate function call overhead and reduce latency.
#pragma HLS inline

  int result;
  result = odd_factorial(n);
  result = even_sum(result);
  return result;
}
int main()
{
  int n = 0xb;
  int ret;
  ret = compute1((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


