
#include <stdio.h>

typedef unsigned char data_t[64];
typedef unsigned int state_t[8];
extern const unsigned int k[64];

void sha256_transform(state_t state, data_t data)
{
#pragma HLS PIPELINE
  unsigned int a;
  unsigned int b;
  unsigned int c;
  unsigned int d;
  unsigned int e;
  unsigned int f;
  unsigned int g;
  unsigned int h;
  unsigned int i;
  unsigned int j;
  unsigned int t1;
  unsigned int t2;
  unsigned int m[64];

  for (i = 0, j = 0; i < 16; ++i, j += 4)
  {
    m[i] = (((data[j] << 24) | (data[j + 1] << 16)) | (data[j + 2] << 8)) | data[j + 3];
  }

  for (; i < 64; ++i)
  {
    unsigned int s0 = (m[i - 15] >> 7) | (m[i - 15] << (32 - 7));
    s0 ^= (m[i - 15] >> 18) | (m[i - 15] << (32 - 18));
    s0 ^= (m[i - 15] >> 3);
    unsigned int s1 = (m[i - 2] >> 17) | (m[i - 2] << (32 - 17));
    s1 ^= (m[i - 2] >> 19) | (m[i - 2] << (32 - 19));
    s1 ^= (m[i - 2] >> 10);
    m[i] = m[i - 16] + s0 + m[i - 7] + s1;
  }

  a = state[0];
  b = state[1];
  c = state[2];
  d = state[3];
  e = state[4];
  f = state[5];
  g = state[6];
  h = state[7];
  
  for (i = 0; i < 64; ++i)
  {
    unsigned int S1 = (e >> 6) | (e << (32 - 6));
    S1 ^= (e >> 11) | (e << (32 - 11));
    S1 ^= (e >> 25) | (e << (32 - 25));
    unsigned int ch = (e & f) ^ ((~e) & g);
    t1 = h + S1 + ch + k[i] + m[i];
    unsigned int S0 = (a >> 2) | (a << (32 - 2));
    S0 ^= (a >> 13) | (a << (32 - 13));
    S0 ^= (a >> 22) | (a << (32 - 22));
    unsigned int maj = (a & b) ^ (a & c) ^ (b & c);
    t2 = S0 + maj;
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  state[0] += a;
  state[1] += b;
  state[2] += c;
  state[3] += d;
  state[4] += e;
  state[5] += f;
  state[6] += g;
  state[7] += h;
}

