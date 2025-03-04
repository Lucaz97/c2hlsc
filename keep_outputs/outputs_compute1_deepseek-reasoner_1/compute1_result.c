
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

int even_sum(int n) {
  if (n <= 2) return 0;
  int m = (n % 2) ? n - 1 : n - 2;
  return (m * (m + 2)) >> 2;
}

int compute1(int n)
{
  int result;
  #pragma HLS inline
  result = odd_factorial(n);
  #pragma HLS inline
  #pragma HLS unroll yes
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


