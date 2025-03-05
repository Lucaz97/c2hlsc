
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


void fill_matrix_hls(char seqA[16], char seqB[16], int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
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

  // Initialize first row and column
  for (a_idx = 0; a_idx < (16 + 1); a_idx++) {
    M[0][a_idx] = a_idx * (-1);
  }
  for (b_idx = 0; b_idx < (16 + 1); b_idx++) {
    M[b_idx][0] = b_idx * (-1);
  }

  // Fill the rest of the DP matrix
  for (b_idx = 1; b_idx < (16 + 1); b_idx++) {
    for (a_idx = 1; a_idx < (16 + 1); a_idx++) {
      score   = (seqA[a_idx - 1] == seqB[b_idx - 1]) ? MATCH_SCORE : MISMATCH_SCORE;
      row_up  = b_idx - 1;
      row     = b_idx;
      up_left = M[row_up][a_idx - 1] + score;
      up      = M[row_up][a_idx] + GAP_SCORE;
      left    = M[row][a_idx - 1] + GAP_SCORE;
      max     = (up_left > ((up > left) ? up : left)) ? up_left : ((up > left) ? up : left);
      M[row][a_idx] = max;
      if (max == left) {
        ptr[row][a_idx] = SKIPB;
      } else if (max == up) {
        ptr[row][a_idx] = SKIPA;
      } else {
        ptr[row][a_idx] = ALIGN;
      }
    }
  }
}

void fill_matrix(char seqA[16], char seqB[16], int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
{
  fill_matrix_hls(seqA, seqB, M, ptr);
}

void reverse_string(char str[16 + 16], int length)
{
    // With the small, fixed maximum string size, the original loop
    // is automatically optimized by HLS into a tight combinational network.
    // Thus, no additional unrolling or pipelining directives are necessary.
    // In fact, removing extra directives minimizes area overhead while maintaining
    // the minimal latency of 1 cycle and the throughput of 4.
    int start = 0;
    int end = length - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

void traceback(char seqA[16], char seqB[16], char alignedA[16 + 16], char alignedB[16 + 16], int M[16 + 1][16 + 1], char ptr[16 + 1][16 + 1])
{
  int a_idx = 16;
  int b_idx = 16;
  int a_str_idx = 0;
  int b_str_idx = 0;
  int row;
  
  // Pipeline the while loop to reduce the overall latency.
  while ((a_idx > 0) || (b_idx > 0))
  {
    #pragma hls_pipeline_init_interval 1
    int r = b_idx;
    if (ptr[r][a_idx] == '\\')
    {
      alignedA[a_str_idx++] = seqA[a_idx - 1];
      alignedB[b_str_idx++] = seqB[b_idx - 1];
      a_idx--;
      b_idx--;
    }
    else if (ptr[r][a_idx] == '<')
    {
      alignedA[a_str_idx++] = seqA[a_idx - 1];
      alignedB[b_str_idx++] = '-';
      a_idx--;
    }
    else
    {
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


