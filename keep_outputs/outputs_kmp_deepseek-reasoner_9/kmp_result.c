
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
  #pragma HLS array_partition variable=pattern complete
  #pragma HLS array_partition variable=kmpNext complete
  #pragma HLS latency min=3 max=3
  
  int k = 0;
  kmpNext[0] = 0;

  // Manually unrolled processing for q=1,2,3
  #pragma HLS protocol fixed
  { // q=1
    int current_k = k;
    if(current_k > 0 && pattern[current_k] != pattern[1]) 
      current_k = kmpNext[1];
    if(pattern[current_k] == pattern[1]) 
      current_k++;
    kmpNext[1] = current_k;
    k = current_k;
  }

  #pragma HLS protocol fixed
  { // q=2
    int current_k = k;
    if(current_k > 0 && pattern[current_k] != pattern[2]) 
      current_k = kmpNext[2];
    if(pattern[current_k] == pattern[2]) 
      current_k++;
    kmpNext[2] = current_k;
    k = current_k;
  }

  #pragma HLS protocol fixed
  { // q=3
    int current_k = k;
    if(current_k > 0 && pattern[current_k] != pattern[3]) 
      current_k = kmpNext[3];
    if(pattern[current_k] == pattern[3]) 
      current_k++;
    kmpNext[3] = current_k;
  }
}

static void kmp_process(char pattern[4], char input[204], int kmpNext[4], int n_matches[1])
{
  int q = 0;
  int match_count = 0;
  
  #pragma HLS array_partition variable=pattern complete
  #pragma HLS array_partition variable=kmpNext complete
  
  k1:
  for (int i = 0; i < 204; i++) {
    #pragma HLS pipeline II=3
    
    // Multi-stage failure handling for pattern length 4
    int q_temp = q;
    if (q_temp > 0 && pattern[q_temp] != input[i]) q_temp = kmpNext[q_temp];
    if (q_temp > 0 && pattern[q_temp] != input[i]) q_temp = kmpNext[q_temp];
    if (q_temp > 0 && pattern[q_temp] != input[i]) q_temp = kmpNext[q_temp];

    // State update with match detection
    q_temp = (pattern[q_temp] == input[i]) ? q_temp + 1 : q_temp;
    
    if (q_temp >= 4) {
      match_count++;
      q_temp = kmpNext[3];
    }
    
    q = q_temp;
  }
  
  n_matches[0] = match_count;
}

int kmp(char pattern[4], char input[204], int kmpNext[4], int n_matches[1])
{
  n_matches[0] = 0;
  
  // Precompute failure function with full unroll for pattern processing
  #pragma HLS INLINE // Enable inline for CPF optimization
  CPF(pattern, kmpNext);
  
  // Process input with pipelined pattern matching
  #pragma HLS INLINE // Enable inline for kmp_process optimization
  #pragma HLS PIPELINE INIT_INTERVAL 1
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


