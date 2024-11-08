#include <stdio.h>

#define Nb 4
#define Nk 4        // The number of 32 bit words in a key.
#define Nr 10       // The number of rounds in AES Cipher.
typedef unsigned char uint8_t;
 #define AES_KEYLEN 16   // Key length in bytes
    #define AES_keyExpSize 176

typedef uint8_t state_t[4][4];
typedef uint8_t round_t[176];
struct AES_ctx
{
  uint8_t RoundKey[AES_keyExpSize];
};
static const uint8_t sbox[256] = {
  //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t Rcon[11] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };

#define getSBoxValue(num) (sbox[(num)])


#define Nb 4
#define Nk 4
#define Nr 10
#define AES_KEYLEN 16
#define AES_keyExpSize 176

typedef unsigned char uint8_t;
typedef uint8_t state_t[4][4];
typedef uint8_t round_t[AES_keyExpSize];

static void AddRoundKey(uint8_t round, state_t state, const round_t RoundKey)
{
    #pragma HLS INLINE
    #pragma HLS ARRAY_PARTITION variable=state complete dim=0
    #pragma HLS ARRAY_PARTITION variable=RoundKey cyclic factor=16 dim=1

    uint8_t i, j;
    for (i = 0; i < 4; ++i)
    {
        #pragma HLS UNROLL
        for (j = 0; j < 4; ++j)
        {
            #pragma HLS UNROLL
            state[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
        }
    }
}


typedef unsigned char uint8_t;
typedef uint8_t state_t[4][4];

static void SubBytes(state_t state)
{
    #pragma hls_unroll yes
    for (uint8_t i = 0; i < 4; ++i)
    {
        #pragma hls_unroll yes
        for (uint8_t j = 0; j < 4; ++j)
        {
            state[j][i] = sbox[state[j][i]];
        }
    }
}


#define Nb 4
#define Nk 4
#define Nr 10
typedef unsigned char uint8_t;
#define AES_KEYLEN 16
#define AES_keyExpSize 176

typedef uint8_t state_t[4][4];
typedef uint8_t round_t[176];

static void ShiftRows(state_t state)
{
    #pragma hls_unroll yes
    for (int i = 1; i < 4; i++) {
        uint8_t temp[4];
        #pragma hls_unroll yes
        for (int j = 0; j < 4; j++) {
            temp[j] = state[j][i];
        }
        
        #pragma hls_unroll yes
        for (int j = 0; j < 4; j++) {
            state[j][i] = temp[(j + i) % 4];
        }
    }
}

static uint8_t xtime(uint8_t x)
{
  return (x << 1) ^ (((x >> 7) & 1) * 0x1b);
}



#define Nb 4
#define Nk 4
#define Nr 10
typedef unsigned char uint8_t;
#define AES_KEYLEN 16
#define AES_keyExpSize 176

typedef uint8_t state_t[4][4];
typedef uint8_t round_t[176];

// Other function declarations and definitions...

static uint8_t xtime(uint8_t x);

static void MixColumns(state_t state)
{
    #pragma hls_unroll yes
    for (uint8_t i = 0; i < 4; ++i)
    {
        #pragma hls_pipeline_init_interval 1
        uint8_t t = state[i][0];
        uint8_t Tmp = ((state[i][0] ^ state[i][1]) ^ state[i][2]) ^ state[i][3];
        uint8_t Tm = state[i][0] ^ state[i][1];
        Tm = xtime(Tm);
        state[i][0] ^= Tm ^ Tmp;
        Tm = state[i][1] ^ state[i][2];
        Tm = xtime(Tm);
        state[i][1] ^= Tm ^ Tmp;
        Tm = state[i][2] ^ state[i][3];
        Tm = xtime(Tm);
        state[i][2] ^= Tm ^ Tmp;
        Tm = state[i][3] ^ t;
        Tm = xtime(Tm);
        state[i][3] ^= Tm ^ Tmp;
    }
}


#define Nb 4
#define Nk 4
#define Nr 10
#define AES_KEYLEN 16
#define AES_keyExpSize 176

typedef unsigned char uint8_t;
typedef uint8_t state_t[4][4];
typedef uint8_t round_t[176];

// Function prototypes
static void AddRoundKey(uint8_t round, state_t state, const round_t RoundKey);
static void SubBytes(state_t state);
static void ShiftRows(state_t state);
static void MixColumns(state_t state);

// Optimized Cipher function
static void Cipher(state_t state, const round_t RoundKey)
{
    #pragma HLS INLINE
    
    uint8_t round = 0;
    AddRoundKey(0, state, RoundKey);
    
    #pragma HLS PIPELINE II=1
    for (round = 1; round <= 10; ++round)
    {
        SubBytes(state);
        ShiftRows(state);
        if (round != 10)
        {
            MixColumns(state);
        }
        AddRoundKey(round, state, RoundKey);
    }
}

// Main function for testing
int main()
{
    state_t state = {{170, 187, 204, 221}, {171, 161, 26, 186}, {176, 193, 210, 228}, {189, 175, 250, 255}};
    const round_t RoundKey = {17, 34, 51, 68, 85, 102, 119, 136, 153, 170, 187, 204, 221, 238, 255, 0, 56, 52, 80, 133, 109, 82, 39, 13, 244, 248, 156, 193, 41, 22, 99, 193, 125, 207, 40, 32, 16, 157, 15, 45, 228, 101, 147, 236, 205, 115, 240, 45, 246, 67, 240, 157, 230, 222, 255, 176, 2, 187, 108, 92, 207, 200, 156, 113, 22, 157, 83, 23, 240, 67, 172, 167, 242, 248, 192, 251, 61, 48, 92, 138, 2, 215, 45, 48, 242, 148, 129, 151, 0, 108, 65, 108, 61, 92, 29, 230, 104, 115, 163, 23, 154, 231, 34, 128, 154, 139, 99, 236, 167, 215, 126, 10, 38, 128, 196, 75, 188, 103, 230, 203, 38, 236, 133, 39, 129, 59, 251, 45, 68, 143, 28, 71, 248, 232, 250, 140, 222, 4, 127, 171, 95, 63, 132, 134, 42, 208, 88, 136, 210, 56, 162, 4, 12, 60, 221, 175, 83, 3, 89, 41, 103, 27, 253, 101, 181, 35, 95, 97, 185, 31, 130, 206, 234, 28, 219, 231};
    
    Cipher(state, RoundKey);
    
    for (int _i = 0; _i < 4; _i++)
    {
        for (int _j = 0; _j < 4; _j++)
        {
            printf("%d ", state[_i][_j]);
        }
        printf("\n");
    }

    for (int _i = 0; _i < 176; _i++)
    {
        printf("%d ", RoundKey[_i]);
    }
    printf("\n");

    return 0;
}