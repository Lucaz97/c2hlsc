
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
  #pragma HLS ARRAY_PARTITION variable=from complete dim=1
  #pragma HLS ARRAY_PARTITION variable=to complete dim=1
  #pragma HLS INLINE off
  #pragma HLS LATENCY min=0 max=0
  
  /* Manual full unrolling with explicit element assignments */
  to[0] = from[0]; to[1] = from[1]; to[2] = from[2]; to[3] = from[3];
  to[4] = from[4]; to[5] = from[5]; to[6] = from[6]; to[7] = from[7];
  to[8] = from[8]; to[9] = from[9];
}

void copyKey(present_key_t *from, present_key_t *to) {
  copyKey_hls(*from, *to);
}

void copyBlock(block_t *from, block_t *to);
void copyBlock_hls(block_t from, block_t to)
{
  int i;
  for (i = 0; i < 8; i++)
  {
#pragma HLS unroll yes
    to[i] = from[i];
  }
}

void copyBlock(block_t *from, block_t *to)
{
  copyBlock_hls(*from, *to);
}

void generateRoundKeys80_hls(present_key_t suppliedKey, keys_t keys) {
    present_key_t key;
    present_key_t newKey;
    unsigned char i;
    unsigned char j;

    copyKey_hls(suppliedKey, key);

    block_t initial_block;
    for (j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++) {
        #pragma HLS unroll yes
        initial_block[j] = key[j];
    }
    copyBlock_hls(initial_block, keys[0]);

    for (i = 1; i < ROUNDS; i++) {
        #pragma HLS pipeline II=1
        for (j = 0; j < PRESENT_80_KEY_SIZE_BYTES; j++) {
            #pragma HLS unroll yes
            newKey[j] = (key[(j + 7) % 10] << 5) | (key[(j + 8) % 10] >> 3);
        }

        copyKey_hls(newKey, key);
        key[0] = (sBox[key[0] >> 4] << 4) | (key[0] & 0xF);
        key[8] ^= i << 7;
        key[7] ^= i >> 1;

        block_t round_block;
        for (j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++) {
            #pragma HLS unroll yes
            round_block[j] = key[j];
        }
        copyBlock_hls(round_block, keys[i]);
    }
}

void generateRoundKeys80(present_key_t *suppliedKey, keys_t *keys) {
    generateRoundKeys80_hls(*suppliedKey, *keys);
}

void addRoundKey_hls(block_t block, round_key_t roundKey)
{
  #pragma HLS ARRAY_PARTITION variable=block complete dim=1
  #pragma HLS ARRAY_PARTITION variable=roundKey complete dim=1
  #pragma HLS INLINE off

  int i; // Changed loop variable type for better HLS handling
  for (i = 0; i < 8; ++i) {
    #pragma HLS UNROLL factor=8
    block[i] ^= roundKey[i];
  }
}

void addRoundKey(block_t *block, round_key_t *roundKey)
{
  addRoundKey_hls(*block, *roundKey);
}

void pLayer_hls(block_t block) {
  unsigned char i;
  unsigned char j;
  unsigned char indexVal;
  unsigned char andVal;
  block_t initial;
  copyBlock_hls(block, initial);
  for (i = 0; i < 8; i++) {
    #pragma hls_unroll yes
    block[i] = 0;
    #pragma hls_unroll yes
    for (j = 0; j < 8; j++) {
      indexVal = (4 * (i % 2)) + (3 - (j >> 1));
      andVal = (8 >> (i >> 1)) << ((j % 2) << 2);
      block[i] |= ((initial[indexVal] & andVal) != 0) << j;
    }
  }
}

void pLayer(block_t *block) {
  pLayer_hls(*block);
}

void present80_encryptBlock_hls(block_t block, present_key_t key)
{
  keys_t roundKeys;
#pragma HLS ARRAY_PARTITION variable=roundKeys complete dim=1  // Parallel key access
#pragma HLS ARRAY_PARTITION variable=block complete dim=1       // Byte-level parallelism
  generateRoundKeys80_hls(key, roundKeys);

  ROUND_LOOP:
  for (unsigned char i = 0; i < ROUNDS - 1; i++)
  {
#pragma HLS UNROLL yes  // Full round parallelism (31 instances)
#pragma HLS latency max=2  // Force tighter timing constraints
    addRoundKey_hls(block, roundKeys[i]);
    
    SBOX_LOOP:
    for (unsigned char j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++)
    {
#pragma HLS UNROLL  // Parallel nibble substitutions
      block[j] = (sBox[block[j] >> 4] << 4) | sBox[block[j] & 0xF];
    }
    
    pLayer_hls(block);  // Should now process partitioned bytes in parallel
  }
  addRoundKey_hls(block, roundKeys[ROUNDS - 1]);
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


