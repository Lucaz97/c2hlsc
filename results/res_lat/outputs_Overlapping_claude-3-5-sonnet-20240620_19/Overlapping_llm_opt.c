#include <stdio.h>
#define N 1056768
int epsilon[N]; // array of 0s and 1s
void Overlapping(int *result, int epsilon_element) {
    #pragma HLS INTERFACE ap_none port=result
    #pragma HLS INTERFACE ap_none port=epsilon_element

    static int i = 0;
    static int j = 0;
    static int k = 0;
    static int W_obs = 0;
    static long long chi2 = 0;
    static unsigned int nu[6] = {0, 0, 0, 0, 0, 0};
    static const char sequence[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    static const int K = 5;
    static const int pi[6] = {2746566, 5386218, 7174570, 9943220, 14198031, 7149752}; // Scaled by 1,000,000
    static int buffer[64] = {0};
    static int buffer_index = 0;
    static int chunk_count = 0;
    
    // Add new element to buffer
    buffer[buffer_index] = epsilon_element;
    buffer_index++;

    if (buffer_index == 64) {
        #pragma HLS pipeline
        for (j = 0; j < 56; j++) {
            int match = 1;
            #pragma HLS unroll factor=9
            for (k = 0; k < 9; k++) {
                if (sequence[k] != buffer[j + k]) {
                    match = 0;
                    break;
                }
            }
            W_obs += match;
        }

        // Shift the buffer
        #pragma HLS unroll factor=8
        for (j = 0; j < 8; j++) {
            buffer[j] = buffer[56 + j];
        }
        buffer_index = 8;
        chunk_count++;

        if (chunk_count == 16) {
            if (W_obs <= 4) {
                nu[W_obs]++;
            } else {
                nu[K]++;
            }

            i++;
            W_obs = 0;
            chunk_count = 0;

            if (i == 1024) {
                chi2 = 0;
                #pragma HLS pipeline
                for (int m = 0; m < 6; m++) {
                    chi2 += ((long long)nu[m] * nu[m] * pi[m]) / 1000000;
                }
                *result = (int)chi2;

                // Reset static variables for potential next use
                i = 0;
                #pragma HLS unroll
                for (int m = 0; m < 6; m++) {
                    nu[m] = 0;
                }
            }
        }
    }
}

int main() {
    int i;
    int epsilon[N];
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    int result;
    for (i = 0; i < N; i++) {
        Overlapping(&result, epsilon[i]);
    }

    printf("result = %d\n", result);
    return 0;
}
