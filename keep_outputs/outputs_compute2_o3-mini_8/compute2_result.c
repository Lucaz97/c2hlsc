
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  // Pipeline the loop and provide a tripcount estimate so that the HLS tool
  // can better optimize the scheduling to reduce latency while keeping area low.
  #pragma hls_pipeline_init_interval 1
  for (int i = 1; i < n; i += 2) {
    #pragma HLS loop_tripcount min=1 max=256 avg=128
    res = res * i;
  }
  return res;
}


int even_sum(int n)
{
    // When n <= 2, the loop would not execute; 
    // our closed-form handles this correctly.
    int k;
    if (n & 1)       // if n is odd
        k = n >> 1;  // floor(n/2)
    else             // if n is even
        k = (n >> 1) - 1;  // n/2 - 1

    // Closed-form sum for even numbers: 2 + 4 + ... + 2*k = k*(k+1)
    return k * (k + 1);
}

int compute2(int n)
{
  int result1;
  int result2;
  #pragma HLS dataflow
  result1 = odd_factorial(n);
  result2 = even_sum(n);
  return result1 - result2;
}
int main()
{
  int n = 0xb;
  int ret;
  ret = compute2((int) n);
  printf("%d\n", ret);
  printf("%d\n", n);
}


