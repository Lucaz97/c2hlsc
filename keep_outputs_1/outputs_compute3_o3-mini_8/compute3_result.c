
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c = 0;

  for (int i = 2; i < n; i++)
  {
    // Pipeline the loop to minimize the initiation interval and improve latency.
    #pragma hls_pipeline_init_interval 1
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int odd_sum(int n)
{
  int sum = 0;
  // Pipeline the loop with an initiation interval of 1 to reduce latency
  #pragma hls_pipeline_init_interval 1
  for (int i = 1; i < n; i += 2)
  {
    sum += i;
  }
  return sum;
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

int compute3(int n)
{
  int result0;
  int result1;
  int result2;
  int result3;

  result0 = fibonacci(n);

  // In order to reduce overall latency, we use a dataflow region so that
  // odd_sum and even_sum execute concurrently.
  #pragma HLS DATAFLOW
  {
    result1 = odd_sum(result0);
    result2 = even_sum(n);
  }

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


