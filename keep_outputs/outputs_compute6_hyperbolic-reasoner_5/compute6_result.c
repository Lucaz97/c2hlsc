
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

int odd_factorial(int n)
{
  int res = 1;
  #pragma HLS unroll yes
  for (int i = 1; i < n; i += 2)
  {
    res = res * i;
  }
  return res;
}

int fibonacci(int n) {
  int a = 0;
  int b = 1;
  int c;
  #pragma HLS pipeline II=1
  for (int i = 2; i < n; i++) {
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

void compute6(int n[5])
{
  int result0;
  int result1;
  int result2;
  int result3;
  for (int i = 0; i < 5; i++)
  {
    #pragma HLS unroll yes
    result0 = odd_factorial(n[i]);
    result1 = fibonacci(n[i]);
    result2 = even_sum(result0 - result1);
    result3 = even_sum(result2 - result1);
    n[i] = result3;
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


