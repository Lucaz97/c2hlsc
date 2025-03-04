
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

int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;

  #pragma hls_unroll yes
  for (int i = 2; i < n; i++)
  {
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int even_sum(int n)
{
  int sum = 0;
  #pragma hls_unroll yes
  for (int i = 2; i < n; i += 2)
  {
    sum += i;
  }

  return sum;
}

int compute4(int n)
{
  int result0;
  int result1;
  int result2;
  int result3;

  #pragma HLS pipeline II=1
  result0 = odd_factorial(n);
  result1 = fibonacci(n);
  result2 = even_sum(result0 - result1);
  result3 = even_sum(result2 - result1);

  return result3;
}
int main()
{
  int n = 0x8;
  int ret;
  ret = compute4((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


