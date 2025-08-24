
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
    #pragma hls_pipeline_init_interval 0
    c = a + b;
    a = b;
    b = c;
  }

  return c;
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

int odd_sum(int n)
{
  int sum = 0;
  #pragma HLS unroll yes
  for (int i = 1; i < n; i += 2)
  {
    sum += i;
  }

  return sum;
}

void compute5(int n[5])
{
  #pragma HLS array_partition variable=n complete dim=1
  for (int i = 0; i < 5; i++)
  {
    #pragma HLS unroll yes
    int result0, result1, result2, result3;
    
    // Optimized execution pattern with staggered start
    #pragma HLS pipeline II=1
    {
      #pragma HLS latency min=1 max=3
      result0 = fibonacci(n[i]);
      
      #pragma HLS latency min=1 max=2
      result2 = even_sum(n[i]);
    }

    #pragma HLS pipeline II=1
    {
      #pragma HLS latency min=1 max=2
      result1 = odd_sum(result0);
      
      #pragma HLS latency min=1 max=3
      result3 = fibonacci(result2);
    }

    #pragma HLS protocol fixed
    #pragma HLS expression_balance
    n[i] = result3 - result1;
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


