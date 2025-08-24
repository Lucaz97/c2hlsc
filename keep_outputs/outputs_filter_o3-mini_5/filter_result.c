
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>


#include <stdio.h>

#define NUM_TAPS 5


void shift(int input, int delay_lane[5], int size)
{
  // Partition the delay_lane array completely so that each element is in a separate register.
  // This helps to break up the combinational path.
  #pragma HLS array_partition variable=delay_lane complete

  // When size is 5 (the typical case), manually unroll the shifting operations
  // to further reduce latency by eliminating loop overhead.
  if (size == 5)
  {
    delay_lane[4] = delay_lane[3];
    delay_lane[3] = delay_lane[2];
    delay_lane[2] = delay_lane[1];
    delay_lane[1] = delay_lane[0];
    delay_lane[0] = input;
  }
  else
  {
    // For any other size, fully unroll the loop to minimize latency.
    for (int i = size - 1; i > 0; i--)
    {
      #pragma HLS unroll
      delay_lane[i] = delay_lane[i - 1];
    }
    delay_lane[0] = input;
  }
}

int mac(int delay_lane[5], int taps[5], int size)
{
  int result = 0;
  // For maximal latency reduction when the full tap length is used,
  // we explicitly implement a tree reduction.
  if (size == NUM_TAPS) {
    int m0 = delay_lane[0] * taps[0];
    int m1 = delay_lane[1] * taps[1];
    int m2 = delay_lane[2] * taps[2];
    int m3 = delay_lane[3] * taps[3];
    int m4 = delay_lane[4] * taps[4];

    // Two-level tree reduction minimizes the addition chain depth.
    int sum0 = m0 + m1;
    int sum1 = m2 + m3;
    result = sum0 + sum1 + m4;
  }
  else {
    // When size is not NUM_TAPS, fully unroll the loop to minimize latency.
    for (int i = 0; i < size; i++)
    {
      #pragma hls_unroll yes
      result += delay_lane[i] * taps[i];
    }
  }
  return result;
}

void fir(int input, int *output, int taps[5])
{
  // Optimize for lowest latency with a balanced adder tree
  // to reduce the combinational delay and reduce area.
  static int delay_lane[5] = {0};
  #pragma HLS array_partition variable=delay_lane complete

  // Capture current delay values for use in MAC and update.
  int d0 = delay_lane[0];
  int d1 = delay_lane[1];
  int d2 = delay_lane[2];
  int d3 = delay_lane[3];
  // Note: delay_lane[4] is not needed for the MAC computation.

  // Compute the multiply–accumulate (MAC) using a balanced adder tree.
  int p0 = input   * taps[0];
  int p1 = d0      * taps[1];
  int p2 = d1      * taps[2];
  int p3 = d2      * taps[3];
  int p4 = d3      * taps[4];

  int s0 = p0 + p1;
  int s1 = p2 + p3;
  int sum = s0 + s1 + p4;  // Balanced accumulation to minimize delay

  // Update the delay line (shift-register behavior)
  delay_lane[4] = d3;
  delay_lane[3] = d2;
  delay_lane[2] = d1;
  delay_lane[1] = d0;
  delay_lane[0] = input;

  *output = sum;
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


