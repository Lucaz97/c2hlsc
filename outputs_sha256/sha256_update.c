void sha256_transform(state_t *state, data_t *data)
{
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
    m[i] = ((((*data)[j] << 24) | ((*data)[j + 1] << 16)) | ((*data)[j + 2] << 8)) | (*data)[j + 3];

  for (; i < 64; ++i)
    m[i] = ((((((m[i - 2] >> 17) | (m[i - 2] << (32 - 17))) ^ ((m[i - 2] >> 19) | (m[i - 2] << (32 - 19)))) ^ (m[i - 2] >> 10)) + m[i - 7]) + ((((m[i - 15] >> 7) | (m[i - 15] << (32 - 7))) ^ ((m[i - 15] >> 18) | (m[i - 15] << (32 - 18)))) ^ (m[i - 15] >> 3))) + m[i - 16];

  a = (*state)[0];
  b = (*state)[1];
  c = (*state)[2];
  d = (*state)[3];
  e = (*state)[4];
  f = (*state)[5];
  g = (*state)[6];
  h = (*state)[7];
  for (i = 0; i < 64; ++i)
  {
    t1 = (((h + ((((e >> 6) | (e << (32 - 6))) ^ ((e >> 11) | (e << (32 - 11)))) ^ ((e >> 25) | (e << (32 - 25))))) + ((e & f) ^ ((~e) & g))) + k[i]) + m[i];
    t2 = ((((a >> 2) | (a << (32 - 2))) ^ ((a >> 13) | (a << (32 - 13)))) ^ ((a >> 22) | (a << (32 - 22)))) + (((a & b) ^ (a & c)) ^ (b & c));
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  (*state)[0] += a;
  (*state)[1] += b;
  (*state)[2] += c;
  (*state)[3] += d;
  (*state)[4] += e;
  (*state)[5] += f;
  (*state)[6] += g;
  (*state)[7] += h;
}


void sha256_update(data_t *data_int, unsigned int *datalen_int, state_t *state, unsigned long long int *bitlen_int, data_t *data, size_t len)
{
  int i;
  for (i = 0; i < len; ++i)
  {
    (*data_int)[*datalen_int] = (*data)[i];
    (*datalen_int)++;
    if ((*datalen_int) == 64)
    {
      sha256_transform(state, data_int);
      *bitlen_int += 512;
      *datalen_int = 0;
    }
  }

}


