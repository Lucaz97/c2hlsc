
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


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

int odd_sum(int n)
{
  int sum = 0;
  #pragma hls_unroll yes
  for (int i = 1; i < n; i += 2)
  {
    sum += i;
  }

  return sum;
}

int even_sum(int n)
{
  int sum = 0;
  #pragma hls_pipeline_init_interval 1
  for (int i = 2; i < n; i += 4)
  {
    sum += i;
    sum += i + 2;
  }

  return sum;
}

void compute5(int n[5])
{
  int result0[5];
  int result1[5];
  int result2[5];
  int result3[5];

  #pragma hls_unroll yes
  for (int i = 0; i < 5; i++)
  {
    result0[i] = fibonacci(n[i]);
    result2[i] = even_sum(n[i]);
  }

  #pragma hls_unroll yes
  for (int i = 0; i < 5; i++)
  {
    result1[i] = odd_sum(result0[i]);
    result3[i] = fibonacci(result2[i]);
  }

  #pragma hls_unroll yes
  for (int i = 0; i < 5; i++)
  {
    n[i] = result3[i] - result1[i];
  }
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


