
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
  // Replace the division by 2 with a right-shift to reduce area while maintaining low latency.
  int k = (n - 1) >> 1;  // (n-1)/2 for positive n
  return k * (k + 1);
}

int compute2(int n)
{
  int result1;
  int result2;
#pragma HLS DATAFLOW
  {
    result1 = odd_factorial(n);
  }
  {
    result2 = even_sum(n);
  }
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


