
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
    int i;
    // Fully unroll the loop to implement a combinational copy,
    // ensuring minimal latency.
    #pragma hls_unroll yes
    for (i = 0; i < PRESENT_80_KEY_SIZE_BYTES; i++)
    {
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
    // Fully inline the copy to achieve minimal latency.
    #pragma hls_inline function
    // Manually unroll the fixed-size copy to avoid loop overhead.
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

void generateRoundKeys80_hls(unsigned char suppliedKey[PRESENT_80_KEY_SIZE_BYTES],
                             unsigned char keys[ROUNDS][ROUND_KEY_SIZE_BYTES])
{
    unsigned char key[PRESENT_80_KEY_SIZE_BYTES];
    unsigned char newKey[PRESENT_80_KEY_SIZE_BYTES];
    unsigned char i;

    copyKey_hls(suppliedKey, key);
    copyBlock_hls(key, keys[0]);

    // Outer loop cannot be unrolled since rounds depend on previous round results.
    for (i = 1; i < ROUNDS; i++)
    {
        #pragma hls_pipeline_init_interval 1
        // Manually unroll the transformation of the 10 bytes to avoid loop overhead and modulo computations.
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

        // Apply S-box substitution on the high nibble.
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

// Optimized for lowest latency: Completely unrolled and inlined.
#pragma HLS inline
void addRoundKey_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES],
                     unsigned char roundKey[ROUND_KEY_SIZE_BYTES])
{
  // Manually unroll the XOR operations to implement combinatorial logic.
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

void pLayer_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES])
{
    unsigned char i, j;
    unsigned char indexVal, andVal;
    unsigned char initial[PRESENT_BLOCK_SIZE_BYTES];

    // Copy the original block into a local buffer.
    copyBlock_hls(block, initial);

    // Fully unroll both loops so that the complete permutation is implemented as pure combinational logic.
    #pragma hls_unroll yes
    for (i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++)
    {
        block[i] = 0;
        #pragma hls_unroll yes
        for (j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++)
        {
            indexVal = (4 * (i % 2)) + (3 - (j >> 1));
            andVal   = (8 >> (i >> 1)) << ((j % 2) << 2);
            block[i] |= ( ((initial[indexVal] & andVal) != 0) << j );
        }
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


