
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>


#include <stdio.h>

#define NUM_TAPS 5


void shift(int input, int delay_lane[5], int size)
{
  #pragma hls_unroll yes  
  for (int i = size - 1; i > 0; i--)
  {
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
    // Inline the function so that the internal loops in shift and mac become visible
    // to the synthesis tool. Also, set the pipeline initiation interval to 0 (i.e. disable pipelining)
    // to favor full unrolling of internal loops and hence lower latency.
#pragma HLS inline
#pragma HLS pipeline_init_interval 0

    // Fully partition the delay_lane array to allow parallel accesses in shift and mac,
    // which contributes to reducing latency.
    static int delay_lane[5] = {0};
#pragma HLS array_partition variable=delay_lane complete dim=1

    shift(input, delay_lane, 5);
    *output = mac(delay_lane, taps, 5);
}

void iir(int input, int *output, int feedforward_taps[5], int feedback_taps[5])
{
    // Optimize for latency by inlining the function calls.
    // This forces the compiler to expose inner loop structures in the
    // child functions (shift and mac) which can then be fully unrolled.
    #pragma HLS inline

    static int input_delay_lane[5] = {};
    static int output_delay_lane[5] = {};

    // Inline the shift to minimize overhead.
    #pragma HLS inline
    shift(input, input_delay_lane, 5);

    // Compute feedforward and feedback contributions.
    // Assuming that mac() contains internal loops, inlining it here
    // may allow its loops to be fully unrolled in subsequent optimization passes.
    int feedforward = mac(input_delay_lane, feedforward_taps, 5);
    int feedback    = mac(output_delay_lane, feedback_taps, 5);

    *output = feedforward + feedback;

    // Inline the final shift call as well.
    #pragma HLS inline
    shift(*output, output_delay_lane, 5);
}

void filter(int input, int *fir_output, int *iir_output)
{
  // Apply dataflow to enable concurrent execution of the FIR and IIR filters,
  // reducing the overall latency.
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


