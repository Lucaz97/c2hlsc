#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#define SHA256_BLOCK_SIZE 32            // SHA256 outputs a 32 byte digest

/****************************** MACROS ******************************/
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

/**************************** VARIABLES *****************************/
static const unsigned int k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

typedef  unsigned char data_t[64];
typedef  unsigned int state_t[8];
typedef struct {
	data_t data;
	unsigned int datalen;
	unsigned long long bitlen;
	state_t state;
} SHA256_CTX;

/*********************** FUNCTION DECLARATIONS **********************/
void sha256_init(SHA256_CTX *ctx);
void sha256_update(data_t *data_int, unsigned int * datalen_int, state_t *state, unsigned long long int *bitlen_int, data_t data[], size_t len);
void sha256_final(SHA256_CTX *ctx, unsigned char hash[]);
void sha256_transform(state_t *state, data_t data[]);

void sha256_transform(unsigned int state[8], unsigned char data[64])
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
    m[i] = ((data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | data[j + 3]);

  for (; i < 64; ++i)
    m[i] = (((((m[i - 2] >> 17) | (m[i - 2] << (32 - 17))) ^ ((m[i - 2] >> 19) | (m[i - 2] << (32 - 19)))) ^ (m[i - 2] >> 10)) + m[i - 7]) + ((((m[i - 15] >> 7) | (m[i - 15] << (32 - 7))) ^ ((m[i - 15] >> 18) | (m[i - 15] << (32 - 18)))) ^ (m[i - 15] >> 3)) + m[i - 16];

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
    t1 = h + ((((e >> 6) | (e << (32 - 6))) ^ ((e >> 11) | (e << (32 - 11))) ^ ((e >> 25) | (e << (32 - 25)))) + ((e & f) ^ ((~e) & g))) + k[i] + m[i];
    t2 = (((a >> 2) | (a << (32 - 2))) ^ ((a >> 13) | (a << (32 - 13))) ^ ((a >> 22) | (a << (32 - 22)))) + ((a & b) ^ (a & c) ^ (b & c));
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



#define SHA256_BLOCK_SIZE 32 // SHA256 outputs a 32 byte digest

typedef unsigned char data_t[64];
typedef unsigned int state_t[8];

void sha256_transform(state_t state, data_t data);

void sha256_update(data_t data_int, unsigned int *datalen_int, state_t state, unsigned long long int *bitlen_int, data_t data, size_t len)
{
    int i;
    for (i = 0; i < len; ++i)
    {
        data_int[*datalen_int] = data[i];
        (*datalen_int)++;
        if ((*datalen_int) == 64)
        {
            sha256_transform(state, data_int);
            *bitlen_int += 512;
            *datalen_int = 0;
        }
    }
}

int main()
{
    data_t data_int = {160, 50, 96, 0, 0, 0, 0, 0, 160, 50, 96, 0, 0, 0, 0, 0, 143, 113, 142, 247, 255, 127, 0, 0, 139, 128, 59, 247, 255, 127, 0, 0, 224, 137, 59, 247, 255, 127, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 232, 1, 0, 0, 0, 0, 0, 0, 29, 0, 0, 0, 0, 0, 0, 0};
    unsigned int datalen_int = 0x0;
    state_t state = {1779033703, 3144134277, 1013904242, 2773480762, 1359893119, 2600822924, 528734635, 1541459225};
    unsigned long long int bitlen_int = 0x0;
    data_t data = {97, 98, 99, 100, 98, 99, 100, 101, 99, 100, 101, 102, 100, 101, 102, 103, 101, 102, 103, 104, 102, 103, 104, 105, 103, 104, 105, 106, 104, 105, 106, 107, 105, 106, 107, 108, 106, 107, 108, 109, 107, 108, 109, 110, 108, 109, 110, 111, 109, 110, 111, 112, 110, 111, 112, 113, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t len = 0x38;
    sha256_update(data_int, &datalen_int, state, &bitlen_int, data, len);
    for (int _i = 0; _i < 64; _i++)
    {
        printf("%d ", data_int[_i]);
    }

    printf("\n");
    printf("%d\n", datalen_int);
    for (int _i = 0; _i < 8; _i++)
    {
        printf("%d ", state[_i]);
    }

    printf("\n");
    printf("%llu\n", bitlen_int);
    for (int _i = 0; _i < 64; _i++)
    {
        printf("%d ", data[_i]);
    }

    printf("\n");
    printf("%zu\n", len);

    return 0;
}
