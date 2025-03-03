
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  #pragma HLS pipeline off
  for (int i = 2; i < n; i++)
  {
    #pragma HLS pipeline II=1
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int odd_sum(int n)
{
  int sum = 0;
  #pragma HLS unroll yes
  for (int i = 1; i < n; i += 2)
  {
    #pragma HLS pipeline
    sum += i;
  }
  return sum;
}

int even_sum(int n) {
  int sum = 0;
  for (int i = 2; i < n; i += 2) {
#pragma HLS unroll yes
    sum += i;
  }
  return sum;
}

int compute3(int n)
{
  int result0, result1, result2, result3;
  
  // Parallelize independent function calls
  result0 = fibonacci(n);
  result2 = even_sum(n);  // Moved up to break dependency chain
  
  // Remaining dependent operations
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


