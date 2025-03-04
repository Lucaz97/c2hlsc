
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>


#include <stdio.h>

#define NUM_TAPS 5


void shift(int input, int delay_lane[5], int size)
{
  // Manual full unroll for NUM_TAPS=5 case
  delay_lane[4] = delay_lane[3];
  delay_lane[3] = delay_lane[2];
  delay_lane[2] = delay_lane[1];
  delay_lane[1] = delay_lane[0];

  delay_lane[0] = input;
}

int mac(int delay_lane[5], int taps[5], int size)
{
  int result = 0;
  for (int i = 0; i < size; i++)
  {
#pragma HLS unroll yes  // Fully unroll to eliminate loop overhead and enable parallel execution
    result += delay_lane[i] * taps[i];
  }

  return result;
}

void fir(int input, int *output, int taps[5])
{
  static int delay_lane[5] = {};
  // Fully unroll shift operation to minimize latency
  #pragma HLS inline
  shift(input, delay_lane, 5);
  
  // Fully unroll MAC operation to minimize latency
  #pragma HLS inline
  *output = mac(delay_lane, taps, 5);
}

void iir(int input, int *output, int feedforward_taps[5], int feedback_taps[5])
{
  #pragma HLS ARRAY_PARTITION variable=feedforward_taps complete
  #pragma HLS ARRAY_PARTITION variable=feedback_taps complete
  static int input_delay_lane[5] = {};
  #pragma HLS ARRAY_PARTITION variable=input_delay_lane complete
  static int output_delay_lane[5] = {};
  #pragma HLS ARRAY_PARTITION variable=output_delay_lane complete

  // Parallel shift network with direct assignments
  int new_input_delay[5];
  #pragma HLS ARRAY_PARTITION variable=new_input_delay complete
  new_input_delay[0] = input;
  new_input_delay[1] = input_delay_lane[0];
  new_input_delay[2] = input_delay_lane[1];
  new_input_delay[3] = input_delay_lane[2];
  new_input_delay[4] = input_delay_lane[3];

  // Optimized adder tree with 2-level reduction
  int feedforward = 
    (new_input_delay[0]*feedforward_taps[0] + 
     new_input_delay[1]*feedforward_taps[1] +
     new_input_delay[2]*feedforward_taps[2]) +
    (new_input_delay[3]*feedforward_taps[3] + 
     new_input_delay[4]*feedforward_taps[4]);

  // Mirror structure for feedback path
  int feedback = 
    (output_delay_lane[0]*feedback_taps[0] + 
     output_delay_lane[1]*feedback_taps[1] +
     output_delay_lane[2]*feedback_taps[2]) +
    (output_delay_lane[3]*feedback_taps[3] + 
     output_delay_lane[4]*feedback_taps[4]);

  *output = feedforward + feedback;

  // Output shift with parallel writes
  int new_output_delay[5];
  #pragma HLS ARRAY_PARTITION variable=new_output_delay complete
  new_output_delay[0] = *output;
  new_output_delay[1] = output_delay_lane[0];
  new_output_delay[2] = output_delay_lane[1];
  new_output_delay[3] = output_delay_lane[2];
  new_output_delay[4] = output_delay_lane[3];

  // Buffer updates
  input_delay_lane[0] = new_input_delay[0];
  input_delay_lane[1] = new_input_delay[1];
  input_delay_lane[2] = new_input_delay[2];
  input_delay_lane[3] = new_input_delay[3];
  input_delay_lane[4] = new_input_delay[4];

  output_delay_lane[0] = new_output_delay[0];
  output_delay_lane[1] = new_output_delay[1];
  output_delay_lane[2] = new_output_delay[2];
  output_delay_lane[3] = new_output_delay[3];
  output_delay_lane[4] = new_output_delay[4];
}

void filter(int input, int *fir_output, int *iir_output)
{
  int fir_taps[5] = {1, 2, 3, 2, 1};
  int feedforward_taps[5] = {1, 2, 3, 2, 1};
  int feedback_taps[5] = {0, 1, -1, 0, 0};
  
  // Inline child functions to expose their internal loops for optimization
  #pragma HLS INLINE
  fir(input, fir_output, fir_taps);
  
  // Force full unrolling of potential loops in IIR implementation
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


