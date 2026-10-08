#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

int16_t barrett_reduce(int16_t a)
{
  int16_t t;
  const int16_t v = ((1 << 26) + Q / 2) / Q;

  t = ((int32_t)v * a + (1 << 25)) >> 26;
  t *= Q;
  return a - t;
}

void invntt(int16_t r[N])
{
  unsigned int start, len, j, k;
  int16_t t, zeta;
  const int16_t f = 1441; // mont^2/128

  k = 127;
  for (len = 2; len <= 128; len <<= 1)
  {
    for (start = 0; start < N; start = j + len)
    {
      zeta = zetas[k--];
      for (j = start; j < start + len; j++)
      {
        t = r[j];
        r[j] = barrett_reduce(t + r[j + len]);
        r[j + len] = r[j + len] - t;
        r[j + len] = fqmul(zeta, r[j + len]);
      }
    }
  }

  for (j = 0; j < 256; j++)
    r[j] = fqmul(r[j], f);
}

void ntt(int16_t r[N])
{
  unsigned int len, start, j, k;
  int16_t t, zeta;

  k = 1;
  for (len = 128; len >= 2; len >>= 1)
  {
    for (start = 0; start < N; start = j + len)
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

static void print_poly(const char *label, const int16_t r[N])
{
  printf("%s:\n", label);
  for (int i = 0; i < 5; i++)
  { // print first 16 entries only
    printf("%6d ", r[i]);
  }
  printf("\n...\n\n");
}

int main(void)
{
  int16_t r[N];
  int16_t r_original[N];

  // 1) Initialize test data
  // Simple pattern: r[i] = i (mod something small)
  for (int i = 0; i < N; i++)
  {
    r[i] = (int16_t)i; // you can also randomize this
  }

  // Save original
  memcpy(r_original, r, sizeof(r));

  print_poly("Original polynomial", r);

  // 2) Forward NTT
  ntt(r);
  print_poly("After NTT", r);

  // 3) Inverse NTT
  invntt(r);
  print_poly("After inverse NTT", r);

  // 4) Compare with original
  int ok = 1;
  for (int i = 0; i < N; i++)
  {
    if (r[i] != r_original[i])
    {
      ok = 0;
      printf("Mismatch at index %d: got %d, expected %d\n",
             i, r[i], r_original[i]);
      break;
    }
  }

  if (ok)
  {
    printf("SUCCESS: invntt(ntt(r)) == original r for this test vector.\n");
  }
  else
  {
    printf("FAIL: invntt(ntt(r)) != original r.\n");
  }

  return 0;
}