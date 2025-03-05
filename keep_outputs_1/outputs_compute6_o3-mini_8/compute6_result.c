
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  for (int i = 1; i < n; i += 2)
  {
    #pragma hls_unroll yes
    res = res * i;
  }
  return res;
}

int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  // Full unroll to reduce latency by computing the iterations as a combinational chain.
  #pragma hls_unroll yes
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
  int sum = 0;
  #pragma hls_unroll yes
  for (int i = 2; i < n; i += 2)
  {
    sum += i;
  }
  return sum;
}

void compute6(int n[5])
{
  int tmp0[5];
  int tmp1[5];
  int tmp2[5];
  int tmp3[5];

  // Enable task-level parallelism among the computation stages.
  #pragma HLS dataflow

  // Stage 1: Compute odd_factorial(n[i]) for each element.
  for (int i = 0; i < 5; i++) {
    #pragma HLS pipeline init_interval 1
    tmp0[i] = odd_factorial(n[i]);
  }

  // Stage 2: Compute fibonacci(n[i]) for each element.
  for (int i = 0; i < 5; i++) {
    #pragma HLS pipeline init_interval 1
    tmp1[i] = fibonacci(n[i]);
  }

  // Stage 3: Compute even_sum(tmp0[i] - tmp1[i]) for each element.
  for (int i = 0; i < 5; i++) {
    #pragma HLS pipeline init_interval 1
    tmp2[i] = even_sum(tmp0[i] - tmp1[i]);
  }

  // Stage 4: Compute even_sum(tmp2[i] - tmp1[i]) for each element.
  for (int i = 0; i < 5; i++) {
    #pragma HLS pipeline init_interval 1
    tmp3[i] = even_sum(tmp2[i] - tmp1[i]);
  }

  // Stage 5: Write back the result to the input array.
  for (int i = 0; i < 5; i++) {
    #pragma HLS pipeline init_interval 1
    n[i] = tmp3[i];
  }
}
int main()
{
  unsigned int n[] = {5, 6, 7, 8, 9};
  compute6((int *) n);
  for (int _i = 0; _i < 5; _i++)
  {
    printf("%d ", n[_i]);
  }

  printf("\n");
}


