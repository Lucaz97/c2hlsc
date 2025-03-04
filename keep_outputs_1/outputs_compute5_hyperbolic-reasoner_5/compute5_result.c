
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>


void compute5(int n[5])
{
  int result0;
  int result1;
  int result2;
  int result3;
  #pragma HLS unroll yes
  for (int i = 0; i < 5; i++)
  {
    result0 = fibonacci(n[i]);
    result1 = odd_sum(result0);
    result2 = even_sum(n[i]);
    result3 = fibonacci(result2);
    n[i] = result3 - result1;
  }
}

int fibonacci(int n)
{
  int a = 0;
  int b = 1;
  int c;
  for (int i = 2; i < n; i++)
  {
    #pragma HLS pipeline II=1
    c = a + b;
    a = b;
    b = c;
  }

  return c;
}

int odd_sum(int n) {
    // Mathematical formula replacement: Sum of first k odds = k^2 where k = n//2
    // Removes loop entirely for constant-time operation (1 multiplication + 1 shift)
    return (n/2) * (n/2);
}

int even_sum(int n) {
  if (n <= 2) return 0;
  int m = (n-1)/2;          // Calculate number of even terms
  return m * (m + 1);       // Sum = n_terms * (first + last)/2
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


