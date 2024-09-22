#include <stdio.h>

#define N 16
#define M 8
int epsilon[N*M]; // array of 0s and 1s


#include <stdio.h>
#include <stdlib.h>

#define N 128

void BlockFrequency(int *epsilon, int *is_random) {
    int i, j;
    int blockSum;
    int sum = 0;
    int pi, v;

    for (i = 0; i < 16; i++) {
        blockSum = 0;
        for (j = 0; j < 8; j++) {
            blockSum += epsilon[j + (i * 8)];
        }
        pi = (blockSum << 8) / 8; // pi = blockSum / 8 in fixed-point (scaled by 256)
        v = pi - (128); // 0.5 in fixed-point (scaled by 256)
        sum += (v * v) >> 8; // v*v in fixed-point (scaled by 256^2), then shift right by 8 to scale it back.
    }

    *is_random = (sum < (0.027 * 256)) ? 1 : 0; // 0.027 in fixed-point (scaled by 256)
}

int main() {
    int epsilon[N];
    int i;
    for (i = 0; i < N; i++) {
        epsilon[i] = (i * 73 % 7) == 0;
    }

    int is_random;
    BlockFrequency(epsilon, &is_random);

    printf("is_random = %d\n", is_random);
    return 0;
}
