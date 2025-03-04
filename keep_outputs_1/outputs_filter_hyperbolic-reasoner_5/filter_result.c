
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>


#include <stdio.h>

#define NUM_TAPS 5


void shift(int input, int delay_lane[5], int size)
{
  #pragma HLS array_partition variable=delay_lane complete
  int temp[4];
  #pragma HLS array_partition variable=temp complete

  // Parallel read of original values
  temp[0] = delay_lane[0];
  temp[1] = delay_lane[1];
  temp[2] = delay_lane[2];
  temp[3] = delay_lane[3];

  // Parallel write of shifted values
  delay_lane[1] = temp[0];
  delay_lane[2] = temp[1];
  delay_lane[3] = temp[2];
  delay_lane[4] = temp[3];

  delay_lane[0] = input;
}

int mac(int delay_lane[5], int taps[5], int size)
{
  int result = 0;
  #pragma hls_unroll yes
  for (int i = 0; i < size; i++)
  {
    result += delay_lane[i] * taps[i];
  }
  return result;
}

void fir(int input, int *output, int taps[5])
{
  #pragma HLS ARRAY_PARTITION variable=taps complete dim=1
  static int delay_lane[5] = {};
  #pragma HLS ARRAY_PARTITION variable=delay_lane complete dim=1

  // Inlined shift operation with full unrolling
  delay_lane[4] = delay_lane[3];
  delay_lane[3] = delay_lane[2];
  delay_lane[2] = delay_lane[1];
  delay_lane[1] = delay_lane[0];
  delay_lane[0] = input;

  // Inlined MAC operation with full unrolling
  int acc = 0;
  acc += delay_lane[0] * taps[0];
  acc += delay_lane[1] * taps[1];
  acc += delay_lane[2] * taps[2];
  acc += delay_lane[3] * taps[3];
  acc += delay_lane[4] * taps[4];
  *output = acc;
}

void iir(int input, int *output, int feedforward_taps[5], int feedback_taps[5])
{
  static int input_delay_lane[5] = {};
  static int output_delay_lane[5] = {};
  
  // Fully unroll shift operations to eliminate loop latency
  #pragma HLS INLINE
  shift(input, input_delay_lane, 5);
  
  // Unroll MAC operations for parallel computation
  #pragma HLS INLINE
  #pragma HLS UNROLL yes
  int feedforward = mac(input_delay_lane, feedforward_taps, 5);
  
  #pragma HLS INLINE
  #pragma HLS UNROLL yes
  int feedback = mac(output_delay_lane, feedback_taps, 5);
  
  *output = feedforward + feedback;
  
  #pragma HLS INLINE
  shift(*output, output_delay_lane, 5);
}

void filter(int input, int *fir_output, int *iir_output)
{
  int fir_taps[5] = {1, 2, 3, 2, 1};
  int feedforward_taps[5] = {1, 2, 3, 2, 1};
  int feedback_taps[5] = {0, 1, -1, 0, 0};
  
  // Inline and unroll loops in FIR
  #pragma HLS INLINE
  #pragma HLS UNROLL yes
  fir(input, fir_output, fir_taps);
  
  // Inline and unroll loops in IIR
  #pragma HLS INLINE
  #pragma HLS UNROLL yes
  iir(input, iir_output, feedforward_taps, feedback_taps);
}
int main()
{
  int input = 0x1;
  unsigned int fir_output[] = {0};
  unsigned int iir_output[] = {0};
  filter((int) input, (int *) fir_output, (int *) iir_output);
  printf("%d\n", input);
  for (int _i = 0; _i < 1; _i++)
  {
    printf("%d ", fir_output[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 1; _i++)
  {
    printf("%d ", iir_output[_i]);
  }

  printf("\n");
}


