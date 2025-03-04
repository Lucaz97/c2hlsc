
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


int fibonacci(int n) {
  int a = 0;
  int b = 1;
  int c;
  
  #pragma HLS unroll yes
  for (int i = 2; i < n; i++) {
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int odd_sum(int n)
{
  int k = n / 2;
  return k * k;
}

int even_sum(int n)
{
  int sum = 0;
  // Closed-form formula replacement (preserves functionality)
  if(n > 2) {
    int m = (n % 2 == 0) ? (n/2 - 1) : ((n-1)/2);
    sum = m * (m + 1);
  }
  return sum;
}

int compute3(int n)
{
  int result0;
  int result1;
  int result2;
  int result3;

  // Independent computations (no data dependencies)
  result0 = fibonacci(n);
  result2 = even_sum(n);

  // Dependent computations (use results from above)
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


