
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
  int k = 0;
  kmpNext[0] = 0;

  // Unroll the computation for each q. Since the pattern length is fixed,
  // we manually replicate the loop with fully unrolled inner while logic.
  
  // q = 1
  {
    int q = 1;
    // Fully unroll the possible iterations of the inner while loop (max 3 iterations).
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    
    if (pattern[k] == pattern[q])
      k++;
    kmpNext[q] = k;
  }
  
  // q = 2
  {
    int q = 2;
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    
    if (pattern[k] == pattern[q])
      k++;
    kmpNext[q] = k;
  }
  
  // q = 3
  {
    int q = 3;
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    if((k > 0) && (pattern[k] != pattern[q])) { k = kmpNext[q]; }
    
    if (pattern[k] == pattern[q])
      k++;
    kmpNext[q] = k;
  }
}

static void kmp_process(char pattern[4], char input[204], int kmpNext[4], int n_matches[1])
{
  // Copy the small arrays into local registers to minimize memory access latency.
  char pat0 = pattern[0];
  char pat1 = pattern[1];
  char pat2 = pattern[2];
  char pat3 = pattern[3];

  int kmp0 = kmpNext[0];
  int kmp1 = kmpNext[1];
  int kmp2 = kmpNext[2];
  int kmp3 = kmpNext[3];

  int q = 0;
  k1:
  for (int i = 0; i < STRING_SIZE; i++)
  {
    #pragma HLS pipeline init_interval=1

    // Cascaded mismatches resolved through combinational conditional logic.
    if ((q > 0) && (((q == 1) && (pat1 != input[i])) ||
                    ((q == 2) && (pat2 != input[i])) ||
                    ((q == 3) && (pat3 != input[i]))))
    {
      if (q == 1)
      {
        q = kmp1;
      }
      else if (q == 2)
      {
        q = kmp2;
        if ((q > 0) && (q == 1) && (pat1 != input[i]))  // Second-level check if q drops to 1.
        {
          q = kmp1;
        }
      }
      else // q == 3
      {
        q = kmp3;
        if ((q > 0) && (q == 2) && (pat2 != input[i]))
        {
          q = kmp2;
          if ((q > 0) && (q == 1) && (pat1 != input[i]))
          {
            q = kmp1;
          }
        }
      }
    }

    // Handle match: if the current input char equals the pattern at index q.
    // Notice that when q is 0 we compare pat0, when 1 we compare pat1, etc.
    if (((q == 0) && (pat0 == input[i])) ||
        ((q == 1) && (pat1 == input[i])) ||
        ((q == 2) && (pat2 == input[i])) ||
        ((q == 3) && (pat3 == input[i])))
    {
      q++;
    }

    // When we reach a full match (q>=PATTERN_SIZE), register the match and update q.
    if (q >= PATTERN_SIZE)
    {
      n_matches[0]++;
      // q is updated using the kmpNext value corresponding to PATTERN_SIZE-1.
      q = kmp3;
    }
  }
}

int kmp(char pattern[4], char input[204], int kmpNext[4], int n_matches[1])
{
  // For the lowest latency, bring the input data into local arrays so that
  // subsequent operations in CPF() and kmp_process() use on-chip registers rather than external memory.
  
  // Local copies of the input arrays
  char local_pattern[4];
  char local_input[204];
  int  local_kmpNext[4];
  int  local_n_matches;
  
  // Partition the local arrays to ensure each element is stored in a register.
  #pragma HLS array_partition variable=local_pattern complete dim=1
  #pragma HLS array_partition variable=local_input complete dim=1
  #pragma HLS array_partition variable=local_kmpNext complete dim=1

  // Copy the pattern into local registers. Fully unrolling this tiny loop minimizes latency.
  for (int i = 0; i < 4; i++) {
    #pragma HLS unroll
    local_pattern[i] = pattern[i];
  }

  // Copy the input string into a local array.
  // We pipeline this loop with an initiation interval of 1 so that the copy finishes quickly.
  for (int i = 0; i < 204; i++) {
    #pragma HLS pipeline II=1
    local_input[i] = input[i];
  }

  // Initialize the match counter locally.
  local_n_matches = 0;

  // Inline the child functions to remove call overhead and expose inner loops for further compiler optimizations.
  #pragma HLS inline region recursive
  CPF(local_pattern, local_kmpNext);
  kmp_process(local_pattern, local_input, local_kmpNext, &local_n_matches);

  // Copy the local result (match count) back to the output variable.
  n_matches[0] = local_n_matches;

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


