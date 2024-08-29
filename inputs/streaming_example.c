// Vector fucntion
#define N 20
#define TAPS 11
int x[N];
void fir(*y) {
    int  c[TAPS] = { 53, 0, −91, 0, 313, 500, 313, 0, −91, 0, 53};
    static int shift_reg[TAPS];
    int acc;
    int i, j;
    acc = 0;
    for (j = 0; j < N; j++) {
        for (i = TAPS − 1; i >= 0; i−−) {
            if (i == 0) {
                acc += x[j] ∗ c[0];
                shift reg[0] = x[j];
            } else {
                shift reg[i] = shift reg[i − 1];
                acc += shift reg[i] ∗ c[i];
            }
        }
    }
    *y = acc;
}

// Streaming function
#define TAPS 11
void fir(int ∗y, int x) { // takes one element of x and produces one element of y at each function call 
    int  c[TAPS] = { 53, 0, −91, 0, 313, 500, 313, 0, −91, 0, 53};
    static int shift_reg[TAPS]; // this needs to be static to be preserved across function calls
    static int acc;
    static int j = 0;
    int i;
    acc = 0;
    for (i = TAPS − 1; i >= 0; i−−) {
        if (i == 0) {
            acc += x ∗ c[0];
            shift reg[0] = x;
        } else {
            shift reg[i] = shift reg[i − 1];
            acc += shift reg[i] ∗ c[i];
        }
    }
    if (j==N) {
        *y = acc;
        j = 0;
    } else {
        j++;
    }
}