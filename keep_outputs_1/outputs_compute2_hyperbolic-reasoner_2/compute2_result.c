
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  for (int i = 1; i < n; i += 2)
  {
    #pragma HLS unroll yes
    res = res * i;
  }

  return res;
}

int even_sum(int n)
{
  int sum = 0;
  for (int i = 2; i < n; i += 2)
  {
#pragma HLS unroll yes
    sum += i;
  }
  return sum;
}

int compute2(int n)
{
  int result1;
  int result2;
  
  // No viable loop optimizations possible at this level
  // Maintain sequential execution pattern for function calls
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


