
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
  int score;
  int up_left;
  int up;
  int left;
  int max;
  int row;
  int row_up;
  int a_idx;
  int b_idx;

  // Initialize the first row of M
  for (a_idx = 0; a_idx < (16 + 1); a_idx++) {
    #pragma hls_unroll yes
    M[0][a_idx] = a_idx * (-1);
  }

  // Initialize the first column of M
  for (b_idx = 0; b_idx < (16 + 1); b_idx++) {
    #pragma hls_unroll yes
    M[b_idx][0] = b_idx * (-1);
  }

  // Fill in the rest of the matrix and record the traceback
  for (b_idx = 1; b_idx < (16 + 1); b_idx++) {
    for (a_idx = 1; a_idx < (16 + 1); a_idx++) {
      #pragma hls_unroll yes
      score   = (seqA[a_idx - 1] == seqB[b_idx - 1]) ? (1) : (-1);
      row_up  = b_idx - 1;
      row     = b_idx;
      up_left = M[row_up][a_idx - 1] + score;
      up      = M[row_up][a_idx] + (-1);
      left    = M[row][a_idx - 1] + (-1);
      max     = (up_left > ((up > left) ? (up) : (left))) ? (up_left) : ((up > left) ? (up) : (left));
      M[row][a_idx] = max;
      if (max == left) {
        ptr[row][a_idx] = '<';
      } else if (max == up) {
        ptr[row][a_idx] = '^';
      } else {
        ptr[row][a_idx] = '\\';
      }
    }
  }
}

void reverse_string(char str[16 + 16], int length)
{
  int n = length >> 1; // n = length/2
  // Fully unroll the loop to minimize latency
  for (int i = 0; i < n; i++) {
    #pragma hls_unroll yes
    char temp = str[i];
    str[i] = str[length - i - 1];
    str[length - i - 1] = temp;
  }
}

void traceback(char seqA[16], char seqB[16], char alignedA[16 + 16], char alignedB[16 + 16], int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
{
  int a_idx = 16;
  int b_idx = 16;
  int a_str_idx = 0;
  int b_str_idx = 0;

  // Since the maximum number of iterations is fixed (at most 32),
  // replace the while loop with a for-loop and fully unroll it to minimize latency.
  // This transformation avoids loop control overhead and creates combinational paths.
  for (int i = 0; i < (16 + 16); i++) {
    #pragma hls_unroll yes
    // Exit early if both indices have reached 0
    if ((a_idx == 0) && (b_idx == 0))
      break;

    int r = b_idx;
    if (ptr[r][a_idx] == '\\') {
      alignedA[a_str_idx++] = seqA[a_idx - 1];
      alignedB[b_str_idx++] = seqB[b_idx - 1];
      a_idx--;
      b_idx--;
    }
    else if (ptr[r][a_idx] == '<') {
      alignedA[a_str_idx++] = seqA[a_idx - 1];
      alignedB[b_str_idx++] = '-';
      a_idx--;
    }
    else {
      alignedA[a_str_idx++] = '-';
      alignedB[b_str_idx++] = seqB[b_idx - 1];
      b_idx--;
    }
  }
  
  reverse_string(alignedA, a_str_idx);
  reverse_string(alignedB, b_str_idx);
}

void needwun(char seqA[16], char seqB[16], 
             char alignedA[16 + 16], char alignedB[16 + 16], 
             int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
{
    // In order to minimize latency it is beneficial to inline the core function calls.
    // Full inlining can remove function call overhead and enable optimizations into the inner loops
    // (for example, loop unrolling or pipelining in fill_matrix and traceback, if implemented there).
    #pragma HLS inline

    fill_matrix(seqA, seqB, M, ptr);
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


