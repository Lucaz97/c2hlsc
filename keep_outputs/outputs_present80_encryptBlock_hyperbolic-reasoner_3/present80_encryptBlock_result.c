
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
  #pragma hls_unroll yes
  for (int i = 0; i < PRESENT_80_KEY_SIZE_BYTES; i++)
  {
    to[i] = from[i];
  }
}

void copyKey(present_key_t *from, present_key_t *to)
{
  copyKey_hls(*from, *to);
}

void copyBlock_hls(unsigned char from[PRESENT_BLOCK_SIZE_BYTES], unsigned char to[PRESENT_BLOCK_SIZE_BYTES])
{
#pragma HLS array_partition variable=from complete
#pragma HLS array_partition variable=to complete
  
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

void generateRoundKeys80_hls(unsigned char suppliedKey[PRESENT_80_KEY_SIZE_BYTES], unsigned char keys[ROUNDS][ROUND_KEY_SIZE_BYTES])
{
  unsigned char key[PRESENT_80_KEY_SIZE_BYTES];
  #pragma HLS array_partition variable=key complete
  unsigned char newKey[PRESENT_80_KEY_SIZE_BYTES];
  #pragma HLS array_partition variable=newKey complete
  unsigned char i;
  
  copyKey_hls(suppliedKey, key);
  copyBlock_hls(key, keys[0]);
  
  for (i = 1; i < 32; i++)
  {
    #pragma HLS pipeline II=1
    // Parallel key rotation using partitioned arrays
    newKey[0] = (key[7] << 5) | (key[8] >> 3);
    newKey[1] = (key[8] << 5) | (key[9] >> 3);
    newKey[2] = (key[9] << 5) | (key[0] >> 3);
    newKey[3] = (key[0] << 5) | (key[1] >> 3);
    newKey[4] = (key[1] << 5) | (key[2] >> 3);
    newKey[5] = (key[2] << 5) | (key[3] >> 3);
    newKey[6] = (key[3] << 5) | (key[4] >> 3);
    newKey[7] = (key[4] << 5) | (key[5] >> 3);
    newKey[8] = (key[5] << 5) | (key[6] >> 3);
    newKey[9] = (key[6] << 5) | (key[7] >> 3);

    copyKey_hls(newKey, key);
    
    // Parallel subkey transformations
    key[0] = (sBox[key[0] >> 4] << 4) | (key[0] & 0xF);
    key[8] ^= i << 7;
    key[7] ^= i >> 1;
    
    copyBlock_hls(key, keys[i]);
  }
}

void generateRoundKeys80(present_key_t *suppliedKey, keys_t *keys)
{
  generateRoundKeys80_hls(*suppliedKey, *keys);
}

void addRoundKey_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES], 
                    unsigned char roundKey[ROUND_KEY_SIZE_BYTES]) 
{
    #pragma HLS ARRAY_PARTITION variable=block complete dim=1
    #pragma HLS ARRAY_PARTITION variable=roundKey complete dim=1
    #pragma HLS PIPELINE II=1
    #pragma HLS LATENCY min=1 max=1
    
    // Manual full unrolling for guaranteed parallelism
    block[0] ^= roundKey[0];
    block[1] ^= roundKey[1];
    block[2] ^= roundKey[2];
    block[3] ^= roundKey[3];
    block[4] ^= roundKey[4];
    block[5] ^= roundKey[5];
    block[6] ^= roundKey[6];
    block[7] ^= roundKey[7];
}

void addRoundKey(block_t *block, round_key_t *roundKey) {
    addRoundKey_hls(*block, *roundKey);
}

void pLayer_hls(block_t block) {
  #pragma HLS ARRAY_PARTITION variable=block complete
  block_t initial;
  #pragma HLS ARRAY_PARTITION variable=initial complete
  
  copyBlock_hls(block, initial);

  for (unsigned char i = 0; i < 8; i++) {
    #pragma HLS UNROLL yes
    unsigned char temp = 0;
    
    // Process all bits in parallel
    temp |= ((initial[(4*(i%2))+3] & (8 >> (i >> 1))) != 0) << 0;
    temp |= ((initial[(4*(i%2))+3] & (8 >> (i >> 1) << 4)) != 0) << 1;
    temp |= ((initial[(4*(i%2))+2] & (8 >> (i >> 1))) != 0) << 2;
    temp |= ((initial[(4*(i%2))+2] & (8 >> (i >> 1) << 4)) != 0) << 3;
    temp |= ((initial[(4*(i%2))+1] & (8 >> (i >> 1))) != 0) << 4;
    temp |= ((initial[(4*(i%2))+1] & (8 >> (i >> 1) << 4)) != 0) << 5;
    temp |= ((initial[(4*(i%2))+0] & (8 >> (i >> 1))) != 0) << 6;
    temp |= ((initial[(4*(i%2))+0] & (8 >> (i >> 1) << 4)) != 0) << 7;

    block[i] = temp;
  }
}

void pLayer(block_t *block) {
  pLayer_hls(*block);
}

void present80_encryptBlock_hls(block_t block, present_key_t key)
{
  keys_t roundKeys;
  unsigned char i;
  unsigned char j;
  
  generateRoundKeys80_hls(key, roundKeys);
  for (i = 0; i < (ROUNDS - 1); i++)
  {
    #pragma HLS pipeline II=1
    addRoundKey_hls(block, roundKeys[i]);
    for (j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++)
    {
      #pragma HLS unroll yes
      block[j] = (sBox[block[j] >> 4] << 4) | sBox[block[j] & 0xF];
    }
    pLayer_hls(block);
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


