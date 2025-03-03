
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
  // Instead of using arrays with loops, we implement the delay lines with explicit registers.
  // This manual unrolling of the shift and MAC operations minimizes loop overhead and synthesis logic,
  // yielding reduced area while preserving the 16-cycle latency.
  static int in_d0 = 0, in_d1 = 0, in_d2 = 0, in_d3 = 0, in_d4 = 0;
  static int out_d0 = 0, out_d1 = 0, out_d2 = 0, out_d3 = 0, out_d4 = 0;

  // Update the input delay registers (shift register)
  // The shifting is fully unrolled and mapped to registers.
  in_d4 = in_d3;
  in_d3 = in_d2;
  in_d2 = in_d1;
  in_d1 = in_d0;
  in_d0 = input;

  // Compute feedforward MAC (unrolled)
  int feedforward = 
      in_d0 * feedforward_taps[0] +
      in_d1 * feedforward_taps[1] +
      in_d2 * feedforward_taps[2] +
      in_d3 * feedforward_taps[3] +
      in_d4 * feedforward_taps[4];

  // Compute feedback MAC (unrolled)
  int feedback =
      out_d0 * feedback_taps[0] +
      out_d1 * feedback_taps[1] +
      out_d2 * feedback_taps[2] +
      out_d3 * feedback_taps[3] +
      out_d4 * feedback_taps[4];

  *output = feedforward + feedback;

  // Update the output delay registers (shift register)
  out_d4 = out_d3;
  out_d3 = out_d2;
  out_d2 = out_d1;
  out_d1 = out_d0;
  out_d0 = *output;
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


