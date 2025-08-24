
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  for (int i = 2; i < n; i++) {
    #pragma HLS pipeline II=1
    #pragma HLS latency min=1 max=2
    b = a + (a = b);
  }
  return b;
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
  #pragma hls_pipeline_init_interval 1
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

  // Execute independent Fibonacci/even_sum in parallel
  #pragma HLS allocation function instances=fibonacci limit=2 // Enable parallel calls
  result0 = fibonacci(n);
  result2 = even_sum(n); 

  // Execute dependent odd_sum/fibonacci in parallel
  #pragma HLS allocation function instances=odd_sum, fibonacci limit=1 // Prevent resource conflict
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


