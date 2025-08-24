
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>


#include <stdio.h>

#define NUM_TAPS 5


void shift(int input, int delay_lane[5], int size)
{
  #pragma HLS function_instantiate variable=size
  delay_lane[4] = delay_lane[3];
  delay_lane[3] = delay_lane[2];
  delay_lane[2] = delay_lane[1];
  delay_lane[1] = delay_lane[0];
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
  static int delay_lane[5] = {};
  #pragma HLS ARRAY_PARTITION variable=delay_lane complete dim=0

  // Optimized shift with reduced register pressure
  delay_lane[4] = delay_lane[3];
  delay_lane[3] = delay_lane[2];
  delay_lane[2] = delay_lane[1];
  delay_lane[1] = delay_lane[0];
  delay_lane[0] = input;

  // Direct feedthrough computation with operand folding
  *output = (delay_lane[0] * taps[0]) 
          + (delay_lane[1] * taps[1] 
          + (delay_lane[2] * taps[2] 
          + (delay_lane[3] * taps[3] 
          +  delay_lane[4] * taps[4])));
}

void iir(int input, int *output, int feedforward_taps[5], int feedback_taps[5])
{
  #pragma HLS ARRAY_PARTITION variable=input_delay_lane complete dim=1
  #pragma HLS ARRAY_PARTITION variable=output_delay_lane complete dim=1
  static int input_delay_lane[5] = {};
  static int output_delay_lane[5] = {};

  // Manual unrolled shift register implementation
  #pragma HLS INLINE
  {
    input_delay_lane[4] = input_delay_lane[3];
    input_delay_lane[3] = input_delay_lane[2];
    input_delay_lane[2] = input_delay_lane[1];
    input_delay_lane[1] = input_delay_lane[0];
    input_delay_lane[0] = input;
  }

  // Optimized parallel MAC with balanced arithmetic
  #pragma HLS INLINE
  #pragma HLS UNROLL yes
  #pragma HLS EXPRESSION_BALANCE
  int feedforward = mac(input_delay_lane, feedforward_taps, 5);

  #pragma HLS INLINE
  #pragma HLS UNROLL yes
  #pragma HLS EXPRESSION_BALANCE
  int feedback = mac(output_delay_lane, feedback_taps, 5);

  *output = feedforward + feedback;

  // Output shift with direct parallel assignments
  #pragma HLS INLINE
  {
    output_delay_lane[4] = output_delay_lane[3];
    output_delay_lane[3] = output_delay_lane[2];
    output_delay_lane[2] = output_delay_lane[1];
    output_delay_lane[1] = output_delay_lane[0];
    output_delay_lane[0] = *output;
  }
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


