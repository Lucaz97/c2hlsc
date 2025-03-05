
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


void copyKey_hls(present_key_t from, present_key_t to)
{
  int i;
  #pragma hls_unroll yes
  for (i = 0; i < 10; i++)
  {
    to[i] = from[i];
  }
}

void copyKey(present_key_t *from, present_key_t *to)
{
  copyKey_hls(*from, *to);
}

void copyBlock_hls(block_t from, block_t to)
{
  #pragma hls_unroll yes
  for (int i = 0; i < 8; i++)
  {
    to[i] = from[i];
  }
}

void copyBlock(block_t *from, block_t *to)
{
  copyBlock_hls(*from, *to);
}

void generateRoundKeys80_hls(present_key_t suppliedKey, keys_t keys)
{
  present_key_t key;
  present_key_t newKey;
  unsigned char i;
  unsigned char j;
  copyKey_hls(suppliedKey, key);
  copyBlock_hls(key, keys[0]);
  
  #pragma HLS PIPELINE II=1
  for (i = 1; i < 32; i++)
  {
    #pragma HLS UNROLL
    for (j = 0; j < 10; j++)
    {
      newKey[j] = (key[(j + 7) % 10] << 5) | (key[(j + 8) % 10] >> 3);
    }

    copyKey_hls(newKey, key);
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

void addRoundKey_hls(block_t block, round_key_t roundKey)
{
  #pragma hls_unroll yes
  for (unsigned char i = 0; i < 8; i++)
  {
    block[i] ^= roundKey[i];
  }
}

void addRoundKey(block_t *block, round_key_t *roundKey)
{
  addRoundKey_hls(*block, *roundKey);
}

void pLayer_hls(block_t block)
{
  unsigned char i;
  unsigned char j;
  unsigned char indexVal;
  unsigned char andVal;
  block_t initial;
  copyBlock_hls(block, initial);

  #pragma hls_pipeline_init_interval 1
  for (i = 0; i < 8; i++)
  {
    block[i] = 0;
    #pragma hls_unroll yes
    for (j = 0; j < 8; j++)
    {
      indexVal = (4 * (i % 2)) + (3 - (j >> 1));
      andVal = (8 >> (i >> 1)) << ((j % 2) << 2);
      block[i] |= ((initial[indexVal] & andVal) != 0) << j;
    }
  }
}

void pLayer(block_t *block)
{
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
    addRoundKey_hls(block, roundKeys[i]);

    for (j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++)
    {
      block[j] = (sBox[block[j] >> 4] << 4) | sBox[block[j] & 0xF];
    }

    pLayer_hls(block);
  }

  addRoundKey_hls(block, roundKeys[ROUNDS - 1]);
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


