
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n) {
  int res = 1;
  #pragma HLS unroll yes
  for (int i = 1; i < n; i += 2) {
    res = res * i;
  }
  return res;
}

int even_sum(int n)
{
  if(n <= 2) return 0;
  const int m = (n-1) >> 1;          // Directly compute number of terms
  return m * (m + 1);                // Sum = m*(m+1) where m = floor((n-1)/2)
}

int compute1(int n)
{
  #pragma HLS INLINE
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


