
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>


#include <stdio.h>

#define NUM_TAPS 5


void shift(int input, int delay_lane[5], int size)
{
  #pragma hls_pipeline_init_interval 1
  for (int i = size - 1; i > 0; i--)
  {
    delay_lane[i] = delay_lane[i - 1];
  }

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
  
  // Unroll the shift operation to reduce latency
  #pragma hls_unroll yes
  shift(input, delay_lane, 5);
  
  // Unroll the MAC operation to reduce latency
  #pragma hls_unroll yes
  *output = mac(delay_lane, taps, 5);
}

void iir(int input, int *output, int feedforward_taps[5], int feedback_taps[5])
{
  static int input_delay_lane[5] = {};
  static int output_delay_lane[5] = {};

  // Unroll the shift operation for input_delay_lane
  #pragma hls_unroll yes
  for (int i = 4; i > 0; i--) {
    input_delay_lane[i] = input_delay_lane[i-1];
  }
  input_delay_lane[0] = input;

  // Unroll the MAC operation for feedforward
  int feedforward = 0;
  #pragma hls_unroll yes
  for (int i = 0; i < 5; i++) {
    feedforward += input_delay_lane[i] * feedforward_taps[i];
  }

  // Unroll the MAC operation for feedback
  int feedback = 0;
  #pragma hls_unroll yes
  for (int i = 0; i < 5; i++) {
    feedback += output_delay_lane[i] * feedback_taps[i];
  }

  *output = feedforward + feedback;

  // Unroll the shift operation for output_delay_lane
  #pragma hls_unroll yes
  for (int i = 4; i > 0; i--) {
    output_delay_lane[i] = output_delay_lane[i-1];
  }
  output_delay_lane[0] = *output;
}

void filter(int input, int *fir_output, int *iir_output)
{
  int fir_taps[5] = {1, 2, 3, 2, 1};
  int feedforward_taps[5] = {1, 2, 3, 2, 1};
  int feedback_taps[5] = {0, 1, -1, 0, 0};

  // Unroll the loops in the FIR filter
  #pragma hls_unroll yes
  fir(input, fir_output, fir_taps);

  // Unroll the loops in the IIR filter
  #pragma hls_unroll yes
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


