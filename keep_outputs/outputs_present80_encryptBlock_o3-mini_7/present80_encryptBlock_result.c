
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
    #pragma HLS inline
    for (int i = 0; i < PRESENT_80_KEY_SIZE_BYTES; i++) {
        to[i] = from[i];
    }
}

void copyKey(present_key_t *from, present_key_t *to)
{
    copyKey_hls(*from, *to);
}

void copyBlock_hls(unsigned char from[PRESENT_BLOCK_SIZE_BYTES],
                   unsigned char to[PRESENT_BLOCK_SIZE_BYTES])
{
    for (int i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++) {
        to[i] = from[i];
    }
}

void copyBlock(block_t *from, block_t *to)
{
    copyBlock_hls(*from, *to);
}

void generateRoundKeys80_hls(unsigned char suppliedKey[PRESENT_80_KEY_SIZE_BYTES],
                             unsigned char keys[ROUNDS][ROUND_KEY_SIZE_BYTES])
{
    unsigned char key[PRESENT_80_KEY_SIZE_BYTES];
    unsigned char newKey[PRESENT_80_KEY_SIZE_BYTES];
    unsigned char i, j;

    copyKey_hls(suppliedKey, key);
    copyBlock_hls(key, keys[0]);

    for (i = 1; i < ROUNDS; i++)
    {
        #pragma hls_pipeline_init_interval 1
        for (j = 0; j < PRESENT_80_KEY_SIZE_BYTES; j++)
        {
            #pragma hls_unroll yes
            newKey[j] = (key[(j + 7) % PRESENT_80_KEY_SIZE_BYTES] << 5) |
                        (key[(j + 8) % PRESENT_80_KEY_SIZE_BYTES] >> 3);
        }
        copyKey_hls(newKey, key);
        key[0] = (sBox[key[0] >> 4] << 4) | (key[0] & 0x0F);
        key[8] ^= i << 7;
        key[7] ^= i >> 1;
        copyBlock_hls(key, keys[i]);
    }
}

void generateRoundKeys80(present_key_t *suppliedKey, keys_t *keys)
{
    generateRoundKeys80_hls((*suppliedKey), (*keys));
}

void addRoundKey_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES],
                     unsigned char roundKey[ROUND_KEY_SIZE_BYTES])
{
    unsigned long long b_val = 0;
    unsigned long long k_val = 0;
    int i;
    for (i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++) {
        b_val |= ((unsigned long long) block[i]) << (8 * i);
        k_val |= ((unsigned long long) roundKey[i]) << (8 * i);
    }
    unsigned long long result = b_val ^ k_val;
    for (i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++) {
        block[i] = (result >> (8 * i)) & 0xFF;
    }
}

void addRoundKey(block_t *block, round_key_t *roundKey)
{
    addRoundKey_hls(*block, *roundKey);
}

void pLayer_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES])
{
  unsigned char i;
  unsigned char j;
  unsigned char indexVal;
  unsigned char andVal;
  unsigned char initial[PRESENT_BLOCK_SIZE_BYTES];
  copyBlock_hls(block, initial);

  outer_loop: for (i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++)
  {
    #pragma hls_pipeline_init_interval 0
    unsigned char tmp = 0;
    inner_loop: for (j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++)
    {
      #pragma hls_unroll yes
      indexVal = (4 * (i % 2)) + (3 - (j >> 1));
      andVal = (8 >> (i >> 1)) << ((j % 2) << 2);
      tmp |= (((initial[indexVal] & andVal) != 0) << j);
    }
    block[i] = tmp;
  }
}

void pLayer(block_t *block)
{
  pLayer_hls(*block);
}

void present80_encryptBlock_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES],
                                unsigned char key[PRESENT_80_KEY_SIZE_BYTES])
{
    keys_t roundKeys;
    unsigned char i, j;

    generateRoundKeys80_hls(key, roundKeys);
    // For latency optimization, unroll the outer round loop completely.
    // This transforms the sequential rounds into a combinatorial chain,
    // thus drastically reducing the number of clock cycles.
    #pragma hls_unroll yes
    for (i = 0; i < (ROUNDS - 1); i++)
    {
        addRoundKey_hls(block, roundKeys[i]);
        // Fully unroll the substitution loop.
        #pragma hls_unroll yes
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


