
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  #pragma hls_pipeline_init_interval 1
  for (int i = 2; i < n; i++)
  {
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int odd_sum(int n)
{
  int sum = 0;
  for (int i = 1; i < n; i += 2)
  {
    #pragma HLS unroll yes
    sum += i;
  }

  return sum;
}

int even_sum(int n)
{
  int sum = 0;
  for (int i = 2; i < n; i += 2)
  {
    #pragma HLS unroll factor=2
    sum += i;
  }

  return sum;
}

int compute3(int n)
{
  int result0;
  int result1;
  int result2;
  int result3;

  // Parallelize independent initial computations
  result0 = fibonacci(n);
  result2 = even_sum(n);

  // Parallelize dependent computations after initial results
  result1 = odd_sum(result0);
  result3 = fibonacci(result2);

  return result3 - result1;
}
int main()
{
  int n = 0xb;
  int ret;
  ret = compute3((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


