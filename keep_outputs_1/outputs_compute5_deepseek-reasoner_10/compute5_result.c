
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


void compute5(int n[5])
{
  int result0;
  int result1;
  int result2;
  int result3;
  for (int i = 0; i < 5; i++)
  {
    #pragma HLS unroll yes
    result0 = fibonacci(n[i]);
    result1 = odd_sum(result0);
    result2 = even_sum(n[i]);
    result3 = fibonacci(result2);
    n[i] = result3 - result1;
  }
}

int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  
  #pragma hls_unroll yes
  for (int i = 2; i < n; i++)
  {
    #pragma hls_pipeline_init_interval 0
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int odd_sum(int n)
{
    int m = (n) >> 1;       // Replace division with bit shift
    return m * m;           // 1-cycle multiply (optimized as square)
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
int main()
{
  unsigned int n[] = {6, 7, 8, 9, 10};
  compute5((int *) n);
  for (int _i = 0; _i < 5; _i++)
  {
    printf("%d ", n[_i]);
  }

  printf("\n");
}


