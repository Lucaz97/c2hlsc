
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
    // For n < 2 the function behavior is not well defined in the original code;
    // to safeguard synthesis we explicitly return n.
    if (n < 2) 
        return n;
    
    int a = 0;
    int b = 1;
    int c;

    // Instead of fully unrolling the loop, which increases area,
    // we apply loop pipelining to meet the best latency.
    #pragma hls_pipeline_init_interval 0
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
  // For latency optimization, fully unroll the loop to reduce iteration overhead.
  for (int i = 1; i < n; i += 2) {
    #pragma hls_unroll yes
    sum += i;
  }
  return sum;
}

int even_sum(int n)
{
  int sum = 0;
  // Fully unroll the loop to reduce latency
  #pragma hls_unroll yes
  for (int i = 2; i < n; i += 2)
  {
      sum += i;
  }
  return sum;
}

void compute5(int n[5])
{
  int odd_results[5];
  int even_results[5];

  // Use dataflow to enable concurrent execution of the independent branches.
  #pragma hls_dataflow
  
  // Compute branch A: fibonacci(n[i]) then odd_sum.
  branch1: for (int i = 0; i < 5; i++) {
    #pragma hls_unroll yes
    int temp_fib = fibonacci(n[i]);
    odd_results[i] = odd_sum(temp_fib);
  }
  
  // Compute branch B: even_sum(n[i]) then fibonacci.
  branch2: for (int i = 0; i < 5; i++) {
    #pragma hls_unroll yes
    int temp_even = even_sum(n[i]);
    even_results[i] = fibonacci(temp_even);
  }
  
  // Combine the results.
  combine: for (int i = 0; i < 5; i++) {
    #pragma hls_unroll yes
    n[i] = even_results[i] - odd_results[i];
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


