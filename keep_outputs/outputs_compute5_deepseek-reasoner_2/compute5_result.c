
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  #pragma HLS pipeline_init_interval 0
  for (int i = 2; i < n; i++)
  {
    #pragma HLS unroll yes
    c = a + b;
    a = b;
    b = c;
  }
  return c;
}

int odd_sum(int n) {
  int k = n / 2;
  return k * k;
}

int even_sum(int n)
{
  if(n <= 2) return 0;
  int m = (n-1)/2;         // Calculate number of even terms
  return m * (m + 1);      // Use arithmetic series formula
}

void compute5(int n[5])
{
  for (int i = 0; i < 5; i++)
  {
    #pragma HLS unroll yes
    int result0 = fibonacci(n[i]);
    int result1 = odd_sum(result0);
    int result2 = even_sum(n[i]);
    int result3 = fibonacci(result2);
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


