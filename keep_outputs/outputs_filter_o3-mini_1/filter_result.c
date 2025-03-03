
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>


#include <stdio.h>

#define NUM_TAPS 5


void shift(int input, int delay_lane[5], int size)
{
  // Fully unroll the loop for lower latency
  for (int i = size - 1; i > 0; i--)
  {
    #pragma hls_unroll yes
    delay_lane[i] = delay_lane[i - 1];
  }
  delay_lane[0] = input;
}

int mac(int delay_lane[5], int taps[5], int size)
{
  int result = 0;
  for (int i = 0; i < size; i++)
  {
    #pragma hls_unroll yes
    result += delay_lane[i] * taps[i];
  }
  return result;
}

void fir(int input, int *output, int taps[5])
{
  // To minimize latency further we remove the loops entirely and do the operations
  // in a fully unrolled, combinational style.
  // We first capture the needed delay values so that we can update the delay line
  // without worrying about sequential dependencies.

  static int delay_lane[5] = {0};
  #pragma HLS array_partition variable=delay_lane complete

  // Latch the old delay values needed for compute and update.
  int d0 = delay_lane[0];
  int d1 = delay_lane[1];
  int d2 = delay_lane[2];
  int d3 = delay_lane[3];
  // Note: delay_lane[4] is not used in the computation (dropped on shift)

  // Compute the MAC with the new delay line values:
  // new delay_lane[0] will be 'input', and for indices 1...4, the value
  // corresponds to the previous delay_lane shifted right.
  int acc = input * taps[0] +
            d0    * taps[1] +
            d1    * taps[2] +
            d2    * taps[3] +
            d3    * taps[4];

  // Update the delay line to reflect the shift-register behavior:
  // The new delay_line = { input, old d0, old d1, old d2, old d3 }
  delay_lane[0] = input;
  delay_lane[1] = d0;
  delay_lane[2] = d1;
  delay_lane[3] = d2;
  delay_lane[4] = d3;

  *output = acc;
}

void iir(int input, int *output, int feedforward_taps[5], int feedback_taps[5])
{
  // Partition the delay lanes to allow concurrent accesses in the inner loops of shift and mac,
  // thereby reducing the latency.
  static int input_delay_lane[5] = {};
  static int output_delay_lane[5] = {};
#pragma HLS array_partition variable=input_delay_lane complete
#pragma HLS array_partition variable=output_delay_lane complete

  // Inline these function calls to enable the tool to fully unroll the loops inside them.
  #pragma HLS inline
  shift(input, input_delay_lane, 5);

  int feedforward = mac(input_delay_lane, feedforward_taps, 5);
  int feedback = mac(output_delay_lane, feedback_taps, 5);

  *output = feedforward + feedback;
  shift(*output, output_delay_lane, 5);
}

void filter(int input, int *fir_output, int *iir_output)
{
  // Enable dataflow to execute the FIR and IIR stages concurrently,
  // reducing the overall latency of the filter function.
  #pragma HLS dataflow

  int fir_taps[NUM_TAPS] = {1, 2, 3, 2, 1};
  int feedforward_taps[NUM_TAPS] = {1, 2, 3, 2, 1};
  int feedback_taps[NUM_TAPS] = {0, 1, -1, 0, 0};

  fir(input, fir_output, fir_taps);
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


