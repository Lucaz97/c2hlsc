#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <mc_scverify.h>

#define N 256
#define Q 3329
#define QINV -3327

const int16_t zetas[128] = {
    -1044, -758, -359, -1517, 1493, 1422, 287, 202,
    -171, 622, 1577, 182, 962, -1202, -1474, 1468,
    573, -1325, 264, 383, -829, 1458, -1602, -130,
    -681, 1017, 732, 608, -1542, 411, -205, -1571,
    1223, 652, -552, 1015, -1293, 1491, -282, -1544,
    516, -8, -320, -666, -1618, -1162, 126, 1469,
    -853, -90, -271, 830, 107, -1421, -247, -951,
    -398, 961, -1508, -725, 448, -1065, 677, -1275,
    -1103, 430, 555, 843, -1251, 871, 1550, 105,
    422, 587, 177, -235, -291, -460, 1574, 1653,
    -246, 778, 1159, -147, -777, 1483, -602, 1119,
    -1590, 644, -872, 349, 418, 329, -156, -75,
    817, 1097, 603, 610, 1322, -1285, -1465, 384,
    -1215, -136, 1218, -1335, -874, 220, -1187, -1659,
    -1185, -1530, -1278, 794, -1510, -854, -870, 478,
    -108, -308, 996, 991, 958, -1460, 1522, 1628};

int16_t montgomery_reduce(int32_t a)
{
    int16_t t;

    t = (int16_t)a * QINV;
    t = (a - (int32_t)t * Q) >> 16;
    return t;
}

static int16_t fqmul(int16_t a, int16_t b)
{
    return montgomery_reduce((int32_t)a * b);
}

void ntt(int16_t r[256])
{
    unsigned int len, start, j, k;
    int16_t t, zeta;

    k = 1;
    for (len = 128; len >= 2; len >>= 1)
    {
        for (start = 0; start < 256; start = j + len)
        {
            zeta = zetas[k++];
            for (j = start; j < start + len; j++)
            {
                t = fqmul(zeta, r[j + len]);
                r[j + len] = r[j] - t;
                r[j] = r[j] + t;
            }
        }
    }
}

static void print_poly(const char *label, const int16_t r[256])
{
    printf("%s:\n", label);
    for (int i = 0; i < N; i++)
    { // print first 16 entries only
        printf("%6d ", r[i]);
    }
    printf("\n...\n\n");
}

static uint32_t xorshift32(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

CCS_MAIN(int argc, char *argv[])
{
    enum
    {
        NUM_TESTS = 1000
    };

    uint32_t rng = 0xC0FFEE01u; // deterministic seed
    int failures = 0;

    for (int tc = 0; tc < NUM_TESTS; tc++)
    {
        int16_t r[N];
        int16_t in[N];

        /* -------------------------------------------------
         * Generate random test polynomial
         * ------------------------------------------------- */
        for (int i = 0; i < N; i++)
        {
            r[i] = (int16_t)(xorshift32(&rng) % Q);
        }

        memcpy(in, r, sizeof(r));

        /* Print a few example cases */
        if (tc < 3)
        {
            print_poly("Input polynomial", r);
        }

        /* -------------------------------------------------
         * Run NTT
         * ------------------------------------------------- */
        CCS_BLOCK(ntt)(r);

        if (tc < 3)
        {
            print_poly("After NTT", r);
        }

        /* -------------------------------------------------
         * Sanity checks
         * ------------------------------------------------- */
        int all_zero = 1;
        for (int i = 0; i < N; i++)
        {
            if (r[i] != 0)
            {
                all_zero = 0;
                break;
            }
        }

        if (all_zero)
        {
            failures++;
            printf("[FAIL] Test %d produced all-zero output\n", tc);
            print_poly("Failing input (preview)", in);
        }
    }

    /* -------------------------------------------------
     * Summary
     * ------------------------------------------------- */
    if (failures == 0)
    {
        printf("PASS: %d/%d NTT tests completed successfully.\n",
               NUM_TESTS, NUM_TESTS);
        CCS_RETURN(0);
    }
    else
    {
        printf("DONE: %d failures out of %d tests.\n",
               failures, NUM_TESTS);
        CCS_RETURN(1);
    }
}
