
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
#pragma HLS array_partition variable=kmpNext complete
#pragma HLS array_partition variable=pattern complete
#pragma HLS latency min=2 max=2

  int k = 0;
  kmpNext[0] = 0;
  
  // Optimized q=1 computation
  int k1 = (pattern[0] == pattern[1]) ? 1 : 0;
  kmpNext[1] = k1;
  
  // Merged q=2/q=3 computation
  int k2_tmp = (k1 > 0 && pattern[k1] != pattern[2]) ? kmpNext[k1] : k1;
  int k2 = (pattern[k2_tmp] == pattern[2]) ? k2_tmp + 1 : k2_tmp;
  
  int k3_tmp = (k2 > 0 && pattern[k2] != pattern[3]) ? kmpNext[k2] : k2;
  int k3 = (pattern[k3_tmp] == pattern[3]) ? k3_tmp + 1 : k3_tmp;

  kmpNext[2] = k2;
  kmpNext[3] = k3;
}

static void kmp_process(char pattern[4], char input[204], int kmpNext[4], int n_matches[1])
{
  #pragma HLS array_partition variable=pattern complete
  #pragma HLS array_partition variable=kmpNext complete
  #pragma HLS pipeline

  int q = 0;
  k1:
  for (int i = 0; i < 204; i++) 
  {
    #pragma HLS LOOP_TRIPCOUNT min=204 max=204
    #pragma HLS latency min=3 max=3 // Force aggressive scheduling
    
    // Parallel state reduction with pattern-aware optimization
    int q_next = (pattern[q] != input[i]) ? (q > 0 ? kmpNext[q] : 0) : q;
    int q_final = (pattern[q_next] != input[i]) ? (q_next > 0 ? kmpNext[q_next] : 0) : q_next;

    // Match progression
    q = (pattern[q_final] == input[i]) ? q_final + 1 : q_final;

    // Match completion with direct transition
    if(q >= 4) {
      #pragma HLS protocol fixed
      n_matches[0]++;
      q = kmpNext[3]; // Precomputed fallback
    }
  }
}

int kmp(char pattern[4], char input[204], int kmpNext[4], int n_matches[1])
{
  n_matches[0] = 0;
  
  // Force inline CPF to expose its internal loop for optimization
  #pragma HLS INLINE
  CPF(pattern, kmpNext); // Pattern size 4: full unroll for latency

  // Force inline kmp_process and pipeline its main processing loop
  #pragma HLS INLINE
  #pragma HLS PIPELINE II=1 // Target throughput for long 204-iteration loop
  kmp_process(pattern, input, kmpNext, n_matches);

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


