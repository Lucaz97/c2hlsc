
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>


#include <stdio.h>

#define ALEN 16
#define BLEN 16

#define MATCH_SCORE 1
#define MISMATCH_SCORE -1
#define GAP_SCORE -1

#define ALIGN '\\'
#define SKIPA '^'
#define SKIPB '<'

#define MAX(A, B) (((A) > (B)) ? (A) : (B))


void fill_matrix(char seqA[16], char seqB[16], int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
{
  // Partition arrays to enable parallel access patterns
  #pragma HLS array_partition variable=M dim=1 complete
  #pragma HLS array_partition variable=ptr dim=1 complete
  #pragma HLS array_partition variable=seqA dim=1 complete
  #pragma HLS array_partition variable=seqB dim=1 complete

  // Unrolled initialization with constant propagation
  #pragma HLS unroll yes
  for (int a_idx = 0; a_idx < (16 + 1); a_idx++) {
    M[0][a_idx] = -a_idx;
  }

  #pragma HLS unroll yes
  for (int b_idx = 0; b_idx < (16 + 1); b_idx++) {
    M[b_idx][0] = -b_idx;
  }

  // Anti-diagonal processing with optimized dependency chain
  for (int d = 2; d <= (16*2); d++) {
    #pragma HLS pipeline II=1
    const int start = (d > 16) ? (d - 16) : 1;
    const int end = (d <= 16) ? (d - 1) : 16;
    
    #pragma HLS unroll
    for (int a_idx = start; a_idx <= end; a_idx++) {
      const int b_idx = d - a_idx;
      const int score = (seqA[a_idx-1] == seqB[b_idx-1]) ? MATCH_SCORE : MISMATCH_SCORE;
      
      const int up_left = M[b_idx-1][a_idx-1] + score;
      const int up = M[b_idx-1][a_idx] + GAP_SCORE;
      const int left = M[b_idx][a_idx-1] + GAP_SCORE;
      
      const int max = MAX(up_left, MAX(up, left));
      
      M[b_idx][a_idx] = max;
      ptr[b_idx][a_idx] = (max == left) ? SKIPB : 
                         ((max == up) ? SKIPA : ALIGN);
    }
  }
}

void reverse_string(char str[16 + 16], int length)
{
  // Fully unroll maximum possible iterations (16 swaps for length=32)
  // Use parallel conditional swaps to minimize latency
  #pragma HLS inline off
  #pragma HLS latency min=1 max=1
  
  SwapLoop:
  for(int i = 0; i < 16; i++) {
    #pragma HLS unroll
    if(i < (length >> 1)) {  // Use bitwise shift for HLS-friendly division
      int end = length-1-i;
      char temp = str[i];
      str[i] = str[end];
      str[end] = temp;
    }
  }
}

void traceback(char seqA[16], char seqB[16], char alignedA[32], char alignedB[32], int M[17][17], char ptr[17][17]) {
    #pragma HLS array_partition variable=ptr dim=2 complete
    #pragma HLS array_partition variable=seqA complete
    #pragma HLS array_partition variable=seqB complete
    #pragma HLS array_partition variable=alignedA cyclic factor=4
    #pragma HLS array_partition variable=alignedB cyclic factor=4

    int a_idx = 16;
    int b_idx = 16;
    int a_str_idx = 0;
    int b_str_idx = 0;

    // Optimized fixed-bound loop with partial unrolling
    #pragma HLS loop_tripcount max=32
    #pragma HLS pipeline II=1
    #pragma HLS unroll factor=4
    for (int i = 0; i < 32; i++) {
        #pragma HLS protocol fixed
        if (!(a_idx > 0 || b_idx > 0)) break;

        char direction = ptr[b_idx][a_idx];

        // Parallel path evaluation with speculative writes
        if (direction == '\\') {
            alignedA[a_str_idx] = seqA[a_idx - 1];
            alignedB[b_str_idx] = seqB[b_idx - 1];
            a_idx--;
            b_idx--;
        } else if (direction == '<') {
            alignedA[a_str_idx] = seqA[a_idx - 1];
            alignedB[b_str_idx] = '-';
            a_idx--;
        } else {
            alignedA[a_str_idx] = '-';
            alignedB[b_str_idx] = seqB[b_idx - 1];
            b_idx--;
        }
        
        // Unified index increment pattern
        a_str_idx++;
        b_str_idx++;
    }

    reverse_string(alignedA, a_str_idx);
    reverse_string(alignedB, b_str_idx);
}

void needwun(char seqA[16], char seqB[16], char alignedA[16 + 16], char alignedB[16 + 16], int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
{
  // Force inline child functions to enable loop optimizations
  #pragma HLS inline
  fill_matrix(seqA, seqB, M, ptr);
  
  #pragma HLS inline
  traceback(seqA, seqB, alignedA, alignedB, M, ptr);
}
int main()
{
  unsigned int seqA[] = {1734571124, 1633771873, 1734436711, 7627111};
  unsigned int seqB[] = {1668571508, 1734828391, 1667720039, 6780007};
  unsigned int alignedA[] = {0, 0, 0, 0, 0, 0, 0, 0};
  unsigned int alignedB[] = {0, 0, 0, 0, 0, 0, 0, 0};
  unsigned int M[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  unsigned int ptr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  needwun((char *) seqA, (char *) seqB, (char *) alignedA, (char *) alignedB, (int (*)[16 + 1]) M, (char (*)[16 + 1]) ptr);
  for (int _i = 0; _i < 4; _i++)
  {
    printf("%d ", seqA[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 4; _i++)
  {
    printf("%d ", seqB[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 8; _i++)
  {
    printf("%d ", alignedA[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 8; _i++)
  {
    printf("%d ", alignedB[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 289; _i++)
  {
    printf("%d ", M[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 72; _i++)
  {
    printf("%d ", ptr[_i]);
  }

  printf("\n");
}


