
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


void copyKey_hls(unsigned char from[PRESENT_80_KEY_SIZE_BYTES], unsigned char to[PRESENT_80_KEY_SIZE_BYTES])
{
  // Manual full unrolling for guaranteed parallelization
  // Eliminates loop control logic and forces element-wise operations
  #pragma HLS inline
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

void copyKey(present_key_t *from, present_key_t *to)
{
  copyKey_hls(*from, *to);
}

void copyBlock_hls(unsigned char from[PRESENT_BLOCK_SIZE_BYTES], unsigned char to[PRESENT_BLOCK_SIZE_BYTES])
{
#pragma HLS ARRAY_PARTITION variable=from complete
#pragma HLS ARRAY_PARTITION variable=to complete
  to[0] = from[0];
  to[1] = from[1];
  to[2] = from[2];
  to[3] = from[3];
  to[4] = from[4];
  to[5] = from[5];
  to[6] = from[6];
  to[7] = from[7];
}

void copyBlock(block_t *from, block_t *to)
{
  copyBlock_hls(*from, *to);
}

void addRoundKey_hls(block_t block, round_key_t roundKey)
{
#pragma HLS ARRAY_PARTITION variable=block complete
#pragma HLS ARRAY_PARTITION variable=roundKey complete

  // Manually unrolled XOR operations
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

  for (unsigned char i = 0; i < 8; i++) {
    #pragma HLS UNROLL
    unsigned char temp = 0;
    for (unsigned char j_pair = 0; j_pair < 4; j_pair++) {
      #pragma HLS UNROLL
      const unsigned char indexVal = (4 * (i & 1)) + (3 - j_pair);
      const unsigned char nibble = i >> 1;
      const unsigned char base_bit = 3 - nibble;
      
      const unsigned char byte = initial[indexVal];
      const unsigned char mask_even = 1 << base_bit;
      const unsigned char mask_odd = 1 << (base_bit + 4);
      
      temp |= ((byte & mask_even) ? 1 : 0) << (j_pair*2);
      temp |= ((byte & mask_odd) ? 1 : 0) << (j_pair*2 + 1);
    }
    block[i] = temp;
  }
}

void pLayer(block_t *block) {
  pLayer_hls(*block);
}

void generateRoundKeys80_hls(unsigned char suppliedKey[PRESENT_80_KEY_SIZE_BYTES], unsigned char keys[ROUNDS][ROUND_KEY_SIZE_BYTES]) {
  unsigned char key[PRESENT_80_KEY_SIZE_BYTES];
  unsigned char newKey[PRESENT_80_KEY_SIZE_BYTES];
  unsigned char i;
  unsigned char j;
  
  copyKey_hls(suppliedKey, key);
  copyBlock_hls(key, keys[0]);
  
  for (i = 1; i < 32; i++) {
    #pragma HLS unroll yes
    for (j = 0; j < 10; j++) {
      newKey[j] = (key[(j + 7) % 10] << 5) | (key[(j + 8) % 10] >> 3);
    }
    
    copyKey_hls(newKey, key);
    key[0] = (sBox[key[0] >> 4] << 4) | (key[0] & 0xF);
    key[8] ^= i << 7;
    key[7] ^= i >> 1;
    copyBlock_hls(key, keys[i]);
  }
}

void generateRoundKeys80(present_key_t *suppliedKey, keys_t *keys) {
  generateRoundKeys80_hls(*suppliedKey, *keys);
}

void present80_encryptBlock_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES], unsigned char key[PRESENT_80_KEY_SIZE_BYTES])
{
  unsigned char roundKeys[ROUNDS][ROUND_KEY_SIZE_BYTES];
  #pragma HLS array_partition variable=roundKeys complete

  generateRoundKeys80_hls(key, roundKeys);
  
  loop_rounds: for (int i = 0; i < (ROUNDS-1); i++) {
    #pragma HLS unroll yes
    addRoundKey_hls(block, roundKeys[i]);
    
    loop_sbox: for (int j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++) {
      #pragma HLS unroll yes
      block[j] = (sBox[block[j] >> 4] << 4) | sBox[block[j] & 0xF];
    }

    pLayer_hls(block);
  }

  addRoundKey_hls(block, roundKeys[ROUNDS-1]);
}

void present80_encryptBlock(block_t *block, present_key_t *key)
{
  block_t block_copy;
  present_key_t key_copy;
  
  copyBlock_hls(*block, block_copy);
  copyKey_hls(*key, key_copy);
  
  present80_encryptBlock_hls(block_copy, key_copy);
  
  copyBlock_hls(block_copy, *block);
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


