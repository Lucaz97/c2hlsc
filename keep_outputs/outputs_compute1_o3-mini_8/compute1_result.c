
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  // The loop is fully unrolled to reduce latency by creating a combinational chain of multiplies.
  for (int i = 1; i < n; i += 2)
  {
    #pragma hls_unroll yes
    res = res * i;
  }
  return res;
}

int even_sum(int n)
{
  // For n <= 2, no even numbers are added.
  if(n <= 2)
    return 0;

  // Instead of iterating, use the closed-form arithmetic progression sum.
  // Determine the number of even numbers below n.
  int k = (n - 1) / 2;
  
  // The sum of the first k even numbers is given by: 2 + 4 + ... + 2k = k*(k+1)
  return k * (k + 1);
}

int compute1(int n)
{
  #pragma HLS inline  // Inline the function to reduce call overhead and minimize latency

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


