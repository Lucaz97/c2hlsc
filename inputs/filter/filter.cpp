

// shift the delay lane
void shift(int input, int delay_lane[NUM_TAPS], int size) {
    for (int i = size - 1; i > 0; i--) {
        delay_lane[i] = delay_lane[i - 1];
    }
    delay_lane[0] = input;
}

// perform multiply-accumulate operation
int mac(int delay_lane[NUM_TAPS], int taps[NUM_TAPS], int size) {
    int result = 0;
    for (int i = 0; i < size; i++) {
        result += delay_lane[i] * taps[i];
    }
    return result;
}

// IIR filter function
void iir(int input, int *output, int feedforward_taps[NUM_TAPS], int feedback_taps[NUM_TAPS]) {
    static int input_delay_lane[NUM_TAPS] = {}; // delay lane for input samples
    static int output_delay_lane[NUM_TAPS] = {};   // delay lane for output samples

    // shift the input delay lane and insert the new input
    shift(input, input_delay_lane, NUM_TAPS);

    // compute feedforward (using input samples)
    int feedforward = mac(input_delay_lane, feedforward_taps, NUM_TAPS);

    // compute feedback (using past output samples)
    int feedback = mac(output_delay_lane, feedback_taps, NUM_TAPS);

    // compute current output
    *output = feedforward + feedback;

    // shift the output delay lane and insert the current output
    shift(*output, output_delay_lane, NUM_TAPS);
}

// FIR filter function
void fir(int input, int *output, int taps[NUM_TAPS]) {
    static int delay_lane[NUM_TAPS] = {}; // delay lane to retain state between calls

    // shift the delay lane and insert the new input
    shift(input, delay_lane, NUM_TAPS);

    // perform multiply-accumulate operation
    *output = mac(delay_lane, taps, NUM_TAPS);
}

void filter(int input, int *fir_output, int *iir_output) {

    // FIR filter coefficients
    int fir_taps[NUM_TAPS] = {1, 2, 3, 2, 1};

    // IIR filter coefficients
    int feedforward_taps[NUM_TAPS] = {1, 2, 3, 2, 1};   // a coefficients
    int feedback_taps[NUM_TAPS] = {0, 1, -1, 0, 0};     // b coefficients

    // apply FIR filter
    fir(input, fir_output, fir_taps);

    // apply IIR filter
    iir(input, iir_output, feedforward_taps, feedback_taps);
}

