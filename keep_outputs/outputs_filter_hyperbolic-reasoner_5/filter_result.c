
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
  for (int i = 0; i < NUM_TAPS; i++)  // Use compile-time constant from define
  {
#pragma HLS unroll yes  // Force full unroll with verified loop bound
    if (i < size) {     // Preserve original logic guard
      result += delay_lane[i] * taps[i];
    }
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
  static int input_delay_lane[5] = {};
  #pragma HLS ARRAY_PARTITION variable=input_delay_lane complete
  static int output_delay_lane[5] = {};
  #pragma HLS ARRAY_PARTITION variable=output_delay_lane complete

  // Fully unroll shift loops to reduce latency
  shift(input, input_delay_lane, 5);
  
  // Unroll MAC loops for parallel computation
  int feedforward = mac(input_delay_lane, feedforward_taps, 5);
  #pragma HLS UNROLL factor=5
  int feedback = mac(output_delay_lane, feedback_taps, 5);
  #pragma HLS UNROLL factor=5
  
  *output = feedforward + feedback;
  shift(*output, output_delay_lane, 5);
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


