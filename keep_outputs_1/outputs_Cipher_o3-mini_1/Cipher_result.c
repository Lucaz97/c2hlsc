
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
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


static void AddRoundKey_hls(uint8_t round, state_t state, const round_t RoundKey) {
  #pragma HLS INLINE
  #pragma HLS ARRAY_PARTITION variable=state complete dim=1
  #pragma HLS ARRAY_PARTITION variable=state complete dim=2
  
  const uint8_t base = round << 4;
  #pragma HLS BIND_OP variable=base op=add impl=fabric
  
  for (uint8_t i = 0; i < 4; ++i) {
    #pragma HLS UNROLL yes
    const uint8_t row_offset = base + (i << 2);
    #pragma HLS BIND_OP variable=row_offset op=add impl=fabric
    
    for (uint8_t j = 0; j < 4; ++j) {
      #pragma HLS UNROLL yes
      #pragma HLS BIND_OP variable=state op=xor impl=fabric
      state[i][j] ^= RoundKey[row_offset + j];
    }
  }
}

static void AddRoundKey(uint8_t round, state_t *state, const round_t *RoundKey) {
  AddRoundKey_hls(round, *state, *RoundKey);
}

static void SubBytes_hls(state_t state) {
  #pragma HLS array_partition variable=state complete
  for (int i = 0; i < 4; ++i) {
    #pragma HLS unroll
    for (int j = 0; j < 4; ++j) {
      #pragma HLS unroll
      state[j][i] = sbox[state[j][i]];
    }
  }
}

static void SubBytes(state_t *state) {
  SubBytes_hls(*state);
}

static void ShiftRows_hls(state_t state) {
  // Column 1 rotation
  const uint8_t c1_0 = state[0][1];
  const uint8_t c1_1 = state[1][1];
  const uint8_t c1_2 = state[2][1];
  const uint8_t c1_3 = state[3][1];
  
  state[0][1] = c1_1;
  state[1][1] = c1_2;
  state[2][1] = c1_3;
  state[3][1] = c1_0;

  // Column 2 swaps
  const uint8_t c2_0 = state[0][2];
  const uint8_t c2_2 = state[2][2];
  state[0][2] = c2_2;
  state[2][2] = c2_0;

  const uint8_t c2_1 = state[1][2];
  const uint8_t c2_3 = state[3][2];
  state[1][2] = c2_3;
  state[3][2] = c2_1;

  // Column 3 rotation
  const uint8_t c3_0 = state[0][3];
  const uint8_t c3_3 = state[3][3];
  const uint8_t c3_2 = state[2][3];
  const uint8_t c3_1 = state[1][3];
  
  state[0][3] = c3_3;
  state[3][3] = c3_2;
  state[2][3] = c3_1;
  state[1][3] = c3_0;
}

static void ShiftRows(state_t *state) {
  ShiftRows_hls(*state);
}

static uint8_t xtime(uint8_t x) {
  // Optimized for latency using bitwise selection instead of multiplication
  return (x << 1) ^ (0x1b & (-(x >> 7)));
}

static void MixColumns_hls(state_t state) {
  #pragma HLS ARRAY_PARTITION variable=state complete dim=1
  #pragma HLS ARRAY_PARTITION variable=state complete dim=2
  for (uint8_t i = 0; i < 4; ++i) {
    #pragma HLS UNROLL
    const uint8_t t = state[i][0];
    const uint8_t Tmp = state[i][0] ^ state[i][1] ^ state[i][2] ^ state[i][3];
    
    // Parallel computation of all transformation terms
    const uint8_t Tm0 = xtime(state[i][0] ^ state[i][1]);
    const uint8_t Tm1 = xtime(state[i][1] ^ state[i][2]);
    const uint8_t Tm2 = xtime(state[i][2] ^ state[i][3]);
    const uint8_t Tm3 = xtime(state[i][3] ^ t);

    // Parallel state updates
    state[i][0] ^= Tm0 ^ Tmp;
    state[i][1] ^= Tm1 ^ Tmp;
    state[i][2] ^= Tm2 ^ Tmp;
    state[i][3] ^= Tm3 ^ Tmp;
  }
}

static void MixColumns(state_t *state) {
  MixColumns_hls(*state);
}

static void Cipher_hls(state_t state, const round_t RoundKey)
{
  AddRoundKey_hls(0, state, RoundKey);

  // Manually unroll rounds 1-9 with explicit sequence
  #pragma HLS inline
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(1, state, RoundKey);
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(2, state, RoundKey);
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(3, state, RoundKey);
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(4, state, RoundKey);
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(5, state, RoundKey);
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(6, state, RoundKey);
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(7, state, RoundKey);
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(8, state, RoundKey);
  SubBytes_hls(state); ShiftRows_hls(state); MixColumns_hls(state); AddRoundKey_hls(9, state, RoundKey);

  // Final round
  SubBytes_hls(state);
  ShiftRows_hls(state);
  AddRoundKey_hls(10, state, RoundKey);
}

static void Cipher(state_t *state, const round_t *RoundKey)
{
  Cipher_hls(*state, *RoundKey);
}
int main()
{
  unsigned int state[] = {3721182122, 3122307499, 3839017392, 4294619069};
  unsigned int RoundKey[] = {1144201745, 2289526357, 3434850969, 16772829, 2236625976, 220680813, 3248290036, 3244496425, 539545469, 755997968, 3969082852, 770733005, 2649768950, 2969558758, 1550629634, 1906100431, 391355670, 2813084656, 4223727858, 2321297469, 808310530, 2541851890, 1816226816, 3860683837, 396587880, 2149771162, 3965946778, 176084903, 1271169062, 3420874684, 663088166, 771439489, 1193054020, 2365253880, 2877228254, 2256813919, 2287521834, 77740242, 2950511628, 693699411, 1711086439, 1633625013, 3464634297, 3889896682};
  Cipher((state_t *) state, (const round_t *) RoundKey);
  for (int _i = 0; _i < 4; _i++)
  {
    printf("%d ", state[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 44; _i++)
  {
    printf("%d ", RoundKey[_i]);
  }

  printf("\n");
}


