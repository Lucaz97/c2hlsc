
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

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
  if(n <= 2) return 0;
  int m = (n-1)/2; // Equivalent to ceil((n-2)/2) for positive n
  return m * (m + 1);
}

int compute2(int n)
{
  int result1;
  int result2;

  // No viable loop/operation targets for HLS pragmas found in compute2 body
  // Child function optimizations would need to be applied within odd_factorial/even_sum
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


