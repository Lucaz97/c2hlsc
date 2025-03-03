
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define PATTERN_SIZE 4
#define STRING_SIZE (204)


void CPF(char pattern[4], int kmpNext[4])
{
  #pragma HLS ARRAY_PARTITION variable=pattern complete dim=1
  #pragma HLS ARRAY_PARTITION variable=kmpNext complete dim=1
  #pragma HLS BIND_STORAGE variable=kmpNext type=ram_2p impl=lutram

  int k = 0;
  kmpNext[0] = 0;
  
  // Manual unroll with fixed depth state machines
  #pragma HLS pipeline II=3
  // q=1 iteration
  int tmp_k = (k > 0 && pattern[k] != pattern[1]) ? kmpNext[k] : k;
  k = (pattern[tmp_k] == pattern[1]) ? tmp_k + 1 : tmp_k;
  kmpNext[1] = k;

  // q=2 iteration  
  tmp_k = (k > 0 && pattern[k] != pattern[2]) ? kmpNext[k] : k;
  k = (pattern[tmp_k] == pattern[2]) ? tmp_k + 1 : tmp_k;
  kmpNext[2] = k;

  // q=3 iteration
  tmp_k = (k > 0 && pattern[k] != pattern[3]) ? kmpNext[k] : k;
  k = (pattern[tmp_k] == pattern[3]) ? tmp_k + 1 : tmp_k;
  kmpNext[3] = k;
}

static void kmp_process(char pattern[4], char input[204], int kmpNext[4], int n_matches[1])
{
  int i;
  int q = 0;
  k1:
  #pragma HLS pipeline II=1
  for (i = 0; i < 204; i++)
  {
    // Manually unroll while loop into maximum 3 iterations (q=3 -> 0)
    if ((q > 0) && (pattern[q] != input[i]))
    {
      q = kmpNext[q];
      if ((q > 0) && (pattern[q] != input[i]))
      {
        q = kmpNext[q];
        if ((q > 0) && (pattern[q] != input[i]))
        {
          q = kmpNext[q];
        }
      }
    }

    if (pattern[q] == input[i])
    {
      q++;
    }
    if (q >= 4)
    {
      n_matches[0]++;
      q = kmpNext[q - 1];
    }
  }
}

int kmp(char pattern[4], char input[204], int kmpNext[4], int n_matches[1])
{
  n_matches[0] = 0;
  // Force inline CPF to expose pattern-length loop (size=3 iterations)
  #pragma HLS INLINE
  CPF(pattern, kmpNext);

  // Force inline kmp_process and optimize main input scan loop
  #pragma HLS INLINE
  kmp_process(pattern, input, kmpNext, n_matches);
  // Pipeline pragma applies to 204-iteration input scan loop after inlining
  #pragma HLS PIPELINE II=1
  
  return 0;
}
int main()
{
  unsigned int pattern[] = {1819047266};
  unsigned int input[] = {1635209812, 1869116275, 1634625908, 1936941422, 1769173857, 1769234798, 1819047266, 1952542319, 1886217588, 1249469044, 1399744623, 1634887779, 1752656750, 1684105327, 1852138850, 1769365864, 1768187758, 1920300147, 1735289186, 1634038372, 1650553709, 1819047266, 1416918383, 1919972178, 1667589221, 1869837157, 1818842994, 1835100524, 1766548301, 2036690030, 1633971809, 1953461100, 1735749480, 1752462440, 1869509729, 1936028272, 1852138601, 1869116276, 1935961205, 1702261349, 1701998445, 1851877492, 1953462132, 1936552549, 1919443795, 1936420449, 1953391984, 1919248500, 1869902693, 1936287846, 1701210476};
  unsigned int kmpNext[] = {0, 0, 0, 0};
  unsigned int n_matches[] = {0};
  int ret;
  ret = kmp((char *) pattern, (char *) input, (int *) kmpNext, (int *) n_matches);
  printf("%d\n", ret);
  for (int _i = 0; _i < 1; _i++)
  {
    printf("%d ", pattern[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 51; _i++)
  {
    printf("%d ", input[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 4; _i++)
  {
    printf("%d ", kmpNext[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 1; _i++)
  {
    printf("%d ", n_matches[_i]);
  }

  printf("\n");
}


