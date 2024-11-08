#include <stdio.h>
#define N 1056768
int epsilon[N]; // array of 0s and 1s

#define N 1056768
extern int epsilon[N]; // Declare epsilon as an external variable

void Overlapping(int *result, int epsilon_element) {
    static int total_index = 0;
    static int i = 0;
    static int j = 0;
    static int W_obs = 0;
    static long long chi2 = 0;
    static unsigned int nu[6] = {0};
    static const char sequence[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
    static const int K = 5;
    static const int pi_scaled[6] = {2746566, 5386218, 7174570, 9943220, 14198031, 7149752};
    static int buffer[9] = {0};

    if (i < 1024) {
        // Shift the buffer and add new element
        for (int k = 0; k < 8; k++) {
            buffer[k] = buffer[k+1];
        }
        buffer[8] = epsilon_element;
        
        if (j >= 8) {  // We have a full 9-element sequence to check
            int match = 1;
            for (int k = 0; k < 9; k++) {
                if (sequence[k] != buffer[k]) {
                    match = 0;
                    break;
                }
            }
            if (match == 1) {
                W_obs++;
            }
        }
        
        j++;
        if (j == 1032) {  // End of a block
            if (W_obs <= 4) {
                nu[W_obs]++;
            } else {
                nu[K]++;
            }
            W_obs = 0;
            j = 0;
            i++;
        }
    }
    
    total_index++;
    
    if (total_index == 1024 * 1032) {
        chi2 = 0;
        for (int m = 0; m < 6; m++) {
            long long temp = nu[m];
            temp *= nu[m];
            temp *= pi_scaled[m];
            chi2 += temp;
        }
        *result = (int)((chi2 + 500000) / 1000000);  // Rounding to nearest integer
    }
}

int main() {
    int result = 0;
    
    // Initialize epsilon array (you mentioned this is already defined in your code)
    // This is just a placeholder initialization
    for (int i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    
    // Call Overlapping function for each element in epsilon
    for (int i = 0; i < N; i++) {
        Overlapping(&result, epsilon[i]);
    }
    
    printf("result = %d\n", result);
    
    return 0;
}
