#include <stdio.h>
#define N 1056768
int epsilon[N]; // array of 0s and 1s

#define N 1056768

// Fixed-point representation using long long
typedef long long fixed_point;

// Convert double to fixed-point
fixed_point double_to_fixed(double d) {
    return (fixed_point)(d * (1LL << 32));
}

// Convert fixed-point to double
double fixed_to_double(fixed_point f) {
    return (double)f / (1LL << 32);
}

void Overlapping(fixed_point *result, int epsilon_element, int *done) {
    #pragma HLS INTERFACE ap_none port=result
    #pragma HLS INTERFACE ap_none port=epsilon_element
    #pragma HLS INTERFACE ap_none port=done
    #pragma HLS INTERFACE ap_ctrl_none port=return

    static int i = 0, j = 0, k = 0, match, epsilon_index = 0;
    static fixed_point W_obs;
    static fixed_point chi2 = 0;
    static unsigned int nu[6] = {0};
    static const int K = 5;
    static const fixed_point pi[6] = {
        double_to_fixed(2.746566),
        double_to_fixed(5.386218),
        double_to_fixed(7.17457),
        double_to_fixed(9.94322),
        double_to_fixed(14.198031),
        double_to_fixed(7.1497515)
    };
    static const char sequence[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    #pragma HLS ARRAY_PARTITION variable=nu complete
    #pragma HLS ARRAY_PARTITION variable=pi complete
    #pragma HLS ARRAY_PARTITION variable=sequence complete

    *done = 0;

    if (i < 1024) {
        if (j == 0 && k == 0) {
            W_obs = 0;
        }
        
        if (j < 1024) {
            if (k < 9) {
                #pragma HLS PIPELINE II=1
                if (k == 0) {
                    match = 1;
                }
                if (sequence[k] != epsilon_element) {
                    match = 0;
                }
                k++;
                epsilon_index++;
            } else {
                if (match == 1) {
                    W_obs += (1LL << 32);  // Increment by 1 in fixed-point
                }
                k = 0;
                j++;
                epsilon_index -= 8;
            }
        } else {
            int W_obs_int = (int)(W_obs >> 32);  // Extract integer part
            if (W_obs_int <= 4) {
                nu[W_obs_int]++;
            } else {
                nu[K]++;
            }
            j = 0;
            i++;
            epsilon_index += 8;
        }
    } else {
        if (j < 6) {
            #pragma HLS PIPELINE II=1
            fixed_point nu_squared = (fixed_point)nu[j] * nu[j] * (1LL << 32);
            chi2 += (nu_squared / (1LL << 32)) * pi[j];  // Fixed-point multiplication
            j++;
        } else {
            *result = chi2;
            *done = 1;
        }
    }
}

// Assuming epsilon is already defined in the main code
extern int epsilon[N];

// Main function for testing
int main() {
    int i;
    for (i = 0 ; i < N; i++){
        epsilon[i] = i*73%7 ==0;
    }
    fixed_point result;
    int done = 0;
    int epsilon_index = 0;
    
    while (!done) {
        Overlapping(&result, epsilon[epsilon_index % N], &done);
        epsilon_index++;
    }

    printf("result = %.0f\n", fixed_to_double(result));
    return 0;
}
