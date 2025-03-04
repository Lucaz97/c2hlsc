
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  #pragma hls_unroll yes
  for (int i = 1; i < n; i += 2)
  {
    res = res * i;
  }

  return res;
}

int even_sum(int n)
{
  // Calculate using arithmetic series formula instead of iterative summation
  // Sum = (number_of_terms) * (first_term + last_term) / 2
  const int k = (n-1)/2;  // Number of even terms <n
  return k * (k + 1);
}

int compute1(int n)
{
#pragma HLS function inline
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


