
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


void compute5(int n[5])
{
  #pragma HLS array_partition variable=n complete dim=1
  for (int i = 0; i < 5; i++)
  {
    #pragma HLS unroll yes
    int result0, result1, result2, result3;
    
    // Force independent operation units with dedicated resources
    #pragma HLS parallel sections independent
    {
      #pragma HLS section resource variables=result0,result1 core=FAST
      #pragma HLS latency max=3
      {
        result0 = fibonacci(n[i]);
        result1 = odd_sum(result0);
      }
      
      #pragma HLS section resource variables=result2,result3 core=FAST
      #pragma HLS latency max=4
      {
        #pragma HLS allocation function instances=even_sum limit=5
        result2 = even_sum(n[i]);
        #pragma HLS allocation function instances=fibonacci limit=10
        result3 = fibonacci(result2);
      }
    }
    
    #pragma HLS expression_balance off
    #pragma HLS protocol fixed
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


