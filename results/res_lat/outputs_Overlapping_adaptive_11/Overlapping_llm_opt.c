#include <stdio.h>
#define N 1056768
int epsilon[N]; // array of 0s and 1s

void Overlapping(int i, unsigned int *nu) {
    int j, k;
    int match;
    double W_obs = 0; // Reset W_obs for each i
    int K = 5;
    char sequence[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};

    for (j = 0; j < 1024; j++) {
        match = 1;
        #pragma hls_pipeline_init_interval 1
        for (k = 0; k < 9; k++) {
            if (sequence[k] != epsilon[((i * 1032) + j) + k]) {
                match = 0;
                break; // Exit early if there's no match
            }
        }

        if (match == 1) {
            W_obs++;
        }
    }

    if (W_obs <= 4) {
        nu[(int) W_obs]++;
    } else {
        nu[K]++;
    }
}

void finalize_result(double *result, unsigned int *nu) {
    double chi2 = 0.0;
    double pi[6] = {2.746566, 5.386218, 7.17457, 9.94322, 14.198031, 7.1497515};

    for (int i = 0; i < 6; i++) {
        chi2 += (nu[i] * nu[i]) * pi[i];
    }

    *result = chi2;
}

int main() {
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = i * 73 % 7 == 0;
    }
    double result;
    unsigned int nu[6] = {0}; // Initialize nu array

    // Call Overlapping for each segment
    for (i = 0; i < N / 1032; i++) {
        Overlapping(i, nu);
    }

    finalize_result(&result, nu);

    printf("result = %.0f\n", result);
    return 0;
}
