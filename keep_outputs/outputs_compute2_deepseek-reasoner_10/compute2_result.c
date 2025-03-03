
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int compute2(int n)
{
  int result1;
  int result2;

  // Fully unroll loops in odd_factorial to minimize its latency
  #pragma HLS inline
  result1 = odd_factorial(n); // Assume loop unrolling is applied via inlining
  
  // Fully unroll loops in even_sum to minimize its latency
  #pragma HLS inline
  result2 = even_sum(n); // Assume loop unrolling is applied via inlining
  
  return result1 - result2;
}

int odd_factorial(int n)
{
  int res = 1;
  #pragma HLS unroll yes
  for (int i = 1; i < n; i += 2)
  {
    res = res * i;
  }

  return res;
}

int even_sum(int n)
{
  int sum = 0;
  #pragma HLS unroll yes
  for (int i = 2; i < n; i += 2)
  {
    sum += i;
  }

  return sum;
}
int main()
{
  int n = 0xb;
  int ret;
  ret = compute2((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


