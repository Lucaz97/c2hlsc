
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
  // Compute the number of even terms below n.
  // m = floor((n-1)/2). The sum of even numbers is then: 2 + 4 + ... + 2*m = 2*(1+2+...+m) = 2*(m*(m+1)/2) = m*(m+1)
  int m = (n - 1) / 2;
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


