
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

int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  #pragma HLS unroll yes
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
  // Mathematical formula replacement eliminates loop dependency
  int m = (n > 1) ? (n-1)/2 : 0;
  return m * (m + 1);
}

int compute4(int n)
{
  int result0;
  int result1;
  int result2;
  int result3;

<<<<<<< HEAD
  // Parallelize independent expensive operations
  #pragma HLS pipeline
  result0 = odd_factorial(n);
  result1 = fibonacci(n);

  // Maintain serial dependency for subsequent operations
  #pragma HLS pipeline
  result2 = even_sum(result0 - result1);
  
  #pragma HLS pipeline
  result3 = even_sum(result2 - result1);
  
=======
  // Parallelize independent operations via artificial loop unrolling
  for(int i = 0; i < 1; i++) {
    #pragma HLS unroll yes
    result0 = odd_factorial(n);
    result1 = fibonacci(n);
  }

  result2 = even_sum(result0 - result1);
  result3 = even_sum(result2 - result1);
>>>>>>> 65176057dc8463100481c25242ba932c42aed150
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


