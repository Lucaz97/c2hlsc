
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  #pragma hls_pipeline_init_interval 1
  for (int i = 1; i < n; i += 2)
  {
    res = res * i;
  }

  return res;
}

int even_sum(int n)
{
  int sum = 0;
  #pragma hls_pipeline_init_interval 1
  for (int i = 2; i < n; i += 2)
  {
    sum += i;
  }

  return sum;
}

int compute1(int n)
{
  int result;
  #pragma HLS inline
  result = odd_factorial(n);
  #pragma HLS inline
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


