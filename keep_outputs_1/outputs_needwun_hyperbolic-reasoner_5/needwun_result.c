
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
  // Array partitioning for parallel access to rows
  #pragma HLS ARRAY_PARTITION variable=M dim=1 complete
  #pragma HLS ARRAY_PARTITION variable=ptr dim=1 complete

  // Initialize first row with full unrolling
  for (int a_idx = 0; a_idx < (16 + 1); a_idx++) {
    #pragma HLS UNROLL
    M[0][a_idx] = a_idx * (-1);
  }

  // Initialize first column with full unrolling
  for (int b_idx = 0; b_idx < (16 + 1); b_idx++) {
    #pragma HLS UNROLL
    M[b_idx][0] = b_idx * (-1);
  }

  // Process matrix with pipelined inner loop
  for (int b_idx = 1; b_idx < (16 + 1); b_idx++) {
    #pragma HLS PIPELINE II=1
    for (int a_idx = 1; a_idx < (16 + 1); a_idx++) {
      #pragma HLS PIPELINE II=1
      
      const int score = (seqA[a_idx-1] == seqB[b_idx-1]) ? MATCH_SCORE : MISMATCH_SCORE;
      const int row_up = b_idx-1;
      const int row = b_idx;

      const int up_left = M[row_up][a_idx-1] + score;
      const int up = M[row_up][a_idx] + GAP_SCORE;
      const int left = M[row][a_idx-1] + GAP_SCORE;

      const int max_val = MAX(up_left, MAX(up, left));

      M[row][a_idx] = max_val;

      // Determine direction pointer
      if (max_val == left) {
        ptr[row][a_idx] = SKIPB;
      } else if (max_val == up) {
        ptr[row][a_idx] = SKIPA;
      } else {
        ptr[row][a_idx] = ALIGN;
      }
    }
  }
}

void reverse_string(char str[16 + 16], int length)
{
  #pragma HLS inline
  #pragma HLS pipeline_init_interval 0
  
  // Convert to maximum fixed iterations pattern for better unrolling
  #pragma HLS unroll yes
  for(int i = 0; i < 16; i++) { // Maximum possible iterations for 32-char array
    if(i < (length >> 1)) {      // Conditional execution based on actual length
      int end_idx = length - 1 - i;
      char temp = str[i];
      str[i] = str[end_idx];
      str[end_idx] = temp;
    }
  }
}

void traceback(char seqA[16], char seqB[16], char alignedA[16 + 16], char alignedB[16 + 16], int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
{
#pragma HLS array_partition variable=ptr complete dim=1
#pragma HLS array_partition variable=alignedA complete
#pragma HLS array_partition variable=alignedB complete
  int a_idx = 16;
  int b_idx = 16;
  int a_str_idx = 0;
  int b_str_idx = 0;

  for (int i = 0; i < 32; i++) {
#pragma HLS pipeline II=1
    if (a_idx == 0 && b_idx == 0) break;
    
    int r = b_idx;
    char direction = ptr[r][a_idx];
    
    if (direction == '\\') {
      alignedA[a_str_idx] = (a_idx > 0) ? seqA[a_idx-1] : '-';
      alignedB[b_str_idx] = (b_idx > 0) ? seqB[b_idx-1] : '-';
      a_idx--;
      b_idx--;
    } else if (direction == '<') {
      alignedA[a_str_idx] = (a_idx > 0) ? seqA[a_idx-1] : '-';
      alignedB[b_str_idx] = '-';
      a_idx--;
    } else {
      alignedA[a_str_idx] = '-';
      alignedB[b_str_idx] = (b_idx > 0) ? seqB[b_idx-1] : '-';
      b_idx--;
    }
    
    a_str_idx++;
    b_str_idx++;
  }

  reverse_string(alignedA, a_str_idx);
  reverse_string(alignedB, b_str_idx);
}

void needwun(char seqA[16], char seqB[16], char alignedA[16 + 16], char alignedB[16 + 16], int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
{
  // Inline and fully unroll matrix filling to eliminate loop overhead
  #pragma HLS inline
  fill_matrix(seqA, seqB, M, ptr);

  // Inline and fully unroll traceback to collapse alignment steps
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


