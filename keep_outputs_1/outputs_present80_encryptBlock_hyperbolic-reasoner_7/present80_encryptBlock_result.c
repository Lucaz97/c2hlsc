
#include "../include/ac_float.h"
#include "../include/ac_fixed.h"
#include <stdint.h>
#include <stdio.h>

#define PRESENT_80_KEY_SIZE_BYTES 10
#define PRESENT_BLOCK_SIZE_BYTES 8
#define ROUNDS               32
#define ROUND_KEY_SIZE_BYTES  8
typedef unsigned char keys_t[ROUNDS][ROUND_KEY_SIZE_BYTES];
typedef unsigned char present_key_t[PRESENT_80_KEY_SIZE_BYTES];
typedef unsigned char round_key_t[ROUND_KEY_SIZE_BYTES];
typedef unsigned char block_t[PRESENT_BLOCK_SIZE_BYTES];
unsigned char sBox[16] = {
    0xC, 0x5, 0x6, 0xB, 0x9, 0x0, 0xA, 0xD, 0x3, 0xE, 0xF, 0x8, 0x4, 0x7, 0x1, 0x2 };

unsigned char sBoxInverse[16] = {
        0x5, 0xE, 0xF, 0x8, 0xC, 0x1, 0x2, 0xD, 0xB, 0x4, 0x6, 0x3, 0x0, 0x7, 0x9, 0xA };


void copyKey_hls(present_key_t from, present_key_t to) {
  // Fully explicit unrolling for guaranteed parallelization
  to[0] = from[0];
  to[1] = from[1];
  to[2] = from[2];
  to[3] = from[3];
  to[4] = from[4];
  to[5] = from[5];
  to[6] = from[6];
  to[7] = from[7];
  to[8] = from[8];
  to[9] = from[9];
}

void copyKey(present_key_t *from, present_key_t *to) {
  copyKey_hls(*from, *to);
}

void copyBlock_hls(block_t from, block_t to) {
#pragma HLS INLINE
  to[0] = from[0];
  to[1] = from[1];
  to[2] = from[2];
  to[3] = from[3];
  to[4] = from[4];
  to[5] = from[5];
  to[6] = from[6];
  to[7] = from[7];
}

void copyBlock(block_t *from, block_t *to) {
  copyBlock_hls(*from, *to);
}

void generateRoundKeys80_hls(present_key_t suppliedKey, keys_t keys) {
  present_key_t key;
  present_key_t newKey;
  unsigned char i;
  unsigned char j;

  copyKey_hls(suppliedKey, key);

  #pragma HLS UNROLL
  for (j = 0; j < 8; j++) {
    keys[0][j] = key[j];
  }

  for (i = 1; i < 32; i++) {
    #pragma HLS PIPELINE II=1
    #pragma HLS UNROLL
    for (j = 0; j < 10; j++) {
      newKey[j] = (key[(j + 7) % 10] << 5) | (key[(j + 8) % 10] >> 3);
    }

    copyKey_hls(newKey, key);
    key[0] = (sBox[key[0] >> 4] << 4) | (key[0] & 0xF);
    key[8] ^= i << 7;
    key[7] ^= i >> 1;

    #pragma HLS UNROLL
    for (j = 0; j < 8; j++) {
      keys[i][j] = key[j];
    }
  }
}

void generateRoundKeys80(present_key_t *suppliedKey, keys_t *keys) {
  generateRoundKeys80_hls(*suppliedKey, *keys);
}

void addRoundKey_hls(block_t block, round_key_t roundKey)
{
#pragma HLS ARRAY_PARTITION variable=block complete dim=1
#pragma HLS ARRAY_PARTITION variable=roundKey complete dim=1
  
  block[0] ^= roundKey[0];
  block[1] ^= roundKey[1];
  block[2] ^= roundKey[2];
  block[3] ^= roundKey[3];
  block[4] ^= roundKey[4];
  block[5] ^= roundKey[5];
  block[6] ^= roundKey[6];
  block[7] ^= roundKey[7];
}

void addRoundKey(block_t *block, round_key_t *roundKey)
{
  addRoundKey_hls(*block, *roundKey);
}

void pLayer_hls(block_t block) {
  block_t initial;
  #pragma HLS ARRAY_PARTITION variable=initial complete
  #pragma HLS ARRAY_PARTITION variable=block complete
  copyBlock_hls(block, initial);

  // Optimized combined LUT with precomputed (index, mask) pairs
  struct { unsigned char idx; unsigned char mask; } PLUT[8][8] = {
    {{3,8}, {3,128}, {2,8}, {2,128}, {1,8}, {1,128}, {0,8}, {0,128}},
    {{7,8}, {7,128}, {6,8}, {6,128}, {5,8}, {5,128}, {4,8}, {4,128}},
    {{3,4}, {3,64},  {2,4}, {2,64},  {1,4}, {1,64},  {0,4}, {0,64}},
    {{7,4}, {7,64},  {6,4}, {6,64},  {5,4}, {5,64},  {4,4}, {4,64}},
    {{3,2}, {3,32},  {2,2}, {2,32},  {1,2}, {1,32},  {0,2}, {0,32}},
    {{7,2}, {7,32},  {6,2}, {6,32},  {5,2}, {5,32},  {4,2}, {4,32}},
    {{3,1}, {3,16},  {2,1}, {2,16},  {1,1}, {1,16},  {0,1}, {0,16}},
    {{7,1}, {7,16},  {6,1}, {6,16},  {5,1}, {5,16},  {4,1}, {4,16}}
  };
  #pragma HLS ARRAY_PARTITION variable=PLUT complete

  #pragma HLS UNROLL yes
  for (unsigned char i = 0; i < 8; i++) {
    // Single-cycle parallel bit composition
    block[i] = 
      ((initial[PLUT[i][0].idx] & PLUT[i][0].mask) != 0) << 0 |
      ((initial[PLUT[i][1].idx] & PLUT[i][1].mask) != 0) << 1 |
      ((initial[PLUT[i][2].idx] & PLUT[i][2].mask) != 0) << 2 |
      ((initial[PLUT[i][3].idx] & PLUT[i][3].mask) != 0) << 3 |
      ((initial[PLUT[i][4].idx] & PLUT[i][4].mask) != 0) << 4 |
      ((initial[PLUT[i][5].idx] & PLUT[i][5].mask) != 0) << 5 |
      ((initial[PLUT[i][6].idx] & PLUT[i][6].mask) != 0) << 6 |
      ((initial[PLUT[i][7].idx] & PLUT[i][7].mask) != 0) << 7;
  }
}

void pLayer(block_t *block) {
  pLayer_hls(*block);
}

void present80_encryptBlock_hls(block_t block, present_key_t key)
{
  #pragma HLS ARRAY_PARTITION variable=block complete
  keys_t roundKeys;
  #pragma HLS ARRAY_PARTITION variable=roundKeys complete dim=1
  #pragma HLS ARRAY_PARTITION variable=sBox complete
  
  generateRoundKeys80_hls(key, roundKeys);
  
  MAIN_LOOP: for (unsigned char i = 0; i < (32-1); i++) 
  {
    #pragma HLS PIPELINE II=1
    #pragma HLS LATENCY min=3 max=3
    
    // Stage 1: Add round key
    addRoundKey_hls(block, roundKeys[i]);
    
    // Stage 2: Sbox substitution with intermediate register
    block_t sbox_stage;
    #pragma HLS ARRAY_PARTITION variable=sbox_stage complete
    SBOX_LAYER: for (int j = 0; j < 8; j++) 
    {
      #pragma HLS UNROLL
      sbox_stage[j] = (sBox[block[j] >> 4] << 4) | sBox[block[j] & 0xF];
    }
    
    // Stage 3: Permutation layer
    pLayer_hls(sbox_stage);
    COPY_LAYER: for (int j = 0; j < 8; j++) 
    {
      #pragma HLS UNROLL
      block[j] = sbox_stage[j];
    }
  }

  addRoundKey_hls(block, roundKeys[32-1]);
}

void present80_encryptBlock(block_t *block, present_key_t *key)
{
  present80_encryptBlock_hls(*block, *key);
}
int main()
{
  unsigned int block[] = {3721182122, 285278190};
  unsigned int key[] = {1732584193, 4023233417};
  present80_encryptBlock((block_t *) block, (present_key_t *) key);
  for (int _i = 0; _i < 2; _i++)
  {
    printf("%d ", block[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 2; _i++)
  {
    printf("%d ", key[_i]);
  }

  printf("\n");
}


