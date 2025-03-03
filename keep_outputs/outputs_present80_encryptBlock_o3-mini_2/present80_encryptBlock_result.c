
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


void copyKey_hls(unsigned char from[PRESENT_80_KEY_SIZE_BYTES],
                 unsigned char to[PRESENT_80_KEY_SIZE_BYTES])
{
    int i;
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
    int i;
    #pragma hls_unroll yes
    for (i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++)
    {
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
    unsigned char i;

    copyKey_hls(suppliedKey, key);
    copyBlock_hls(key, keys[0]);

    // Optimize for latency: Pipeline the outer loop and remove modulo computations in the inner loop.
    for (i = 1; i < ROUNDS; i++)
    {
#pragma hls_pipeline_init_interval 0
        // Manually unrolled computation of newKey eliminates modulo overhead
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
        key[0] = (sBox[key[0] >> 4] << 4) | (key[0] & 0xF);
        key[8] ^= i << 7;
        key[7] ^= i >> 1;
        copyBlock_hls(key, keys[i]);
    }
}

// DO NOT modify this function; it is used for testing purposes.
void generateRoundKeys80(present_key_t *suppliedKey, keys_t *keys)
{
    generateRoundKeys80_hls(*suppliedKey, *keys);
}

void addRoundKey_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES], 
                     unsigned char roundKey[ROUND_KEY_SIZE_BYTES])
{
    unsigned char i;
    #pragma hls_unroll yes
    for (i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++)
    {
        block[i] ^= roundKey[i];
    }
}

void addRoundKey(block_t *block, round_key_t *roundKey)
{
    addRoundKey_hls(*block, *roundKey);
}

void pLayer_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES])
{
    unsigned char initial[PRESENT_BLOCK_SIZE_BYTES];
    
    // Copy input block into a temporary buffer
    copyBlock_hls(block, initial);

    /* 
      Fully unroll the permutation by expressing each byte's value
      as a fixed combination of bits from the 'initial' array.
      This removes the loop overhead and fixed index computations,
      reducing the critical path delay and hence the latency.
    */

    // Block[0]: using initial[3], initial[2], initial[1], initial[0]
    block[0] =
         (((initial[3] &  8)  ? 1 : 0) << 0) |
         (((initial[3] &128)  ? 1 : 0) << 1) |
         (((initial[2] &  8)  ? 1 : 0) << 2) |
         (((initial[2] &128)  ? 1 : 0) << 3) |
         (((initial[1] &  8)  ? 1 : 0) << 4) |
         (((initial[1] &128)  ? 1 : 0) << 5) |
         (((initial[0] &  8)  ? 1 : 0) << 6) |
         (((initial[0] &128)  ? 1 : 0) << 7);

    // Block[1]: using initial[7], initial[6], initial[5], initial[4]
    block[1] =
         (((initial[7] &  8)  ? 1 : 0) << 0) |
         (((initial[7] &128)  ? 1 : 0) << 1) |
         (((initial[6] &  8)  ? 1 : 0) << 2) |
         (((initial[6] &128)  ? 1 : 0) << 3) |
         (((initial[5] &  8)  ? 1 : 0) << 4) |
         (((initial[5] &128)  ? 1 : 0) << 5) |
         (((initial[4] &  8)  ? 1 : 0) << 6) |
         (((initial[4] &128)  ? 1 : 0) << 7);

    // Block[2]: using initial[3], initial[2], initial[1], initial[0] with reduced mask value 4 (4 and 64)
    block[2] =
         (((initial[3] &  4)  ? 1 : 0) << 0) |
         (((initial[3] & 64)  ? 1 : 0) << 1) |
         (((initial[2] &  4)  ? 1 : 0) << 2) |
         (((initial[2] & 64)  ? 1 : 0) << 3) |
         (((initial[1] &  4)  ? 1 : 0) << 4) |
         (((initial[1] & 64)  ? 1 : 0) << 5) |
         (((initial[0] &  4)  ? 1 : 0) << 6) |
         (((initial[0] & 64)  ? 1 : 0) << 7);

    // Block[3]: using initial[7], initial[6], initial[5], initial[4] with mask 4 and 64
    block[3] =
         (((initial[7] &  4)  ? 1 : 0) << 0) |
         (((initial[7] & 64)  ? 1 : 0) << 1) |
         (((initial[6] &  4)  ? 1 : 0) << 2) |
         (((initial[6] & 64)  ? 1 : 0) << 3) |
         (((initial[5] &  4)  ? 1 : 0) << 4) |
         (((initial[5] & 64)  ? 1 : 0) << 5) |
         (((initial[4] &  4)  ? 1 : 0) << 6) |
         (((initial[4] & 64)  ? 1 : 0) << 7);

    // Block[4]: using initial[3], initial[2], initial[1], initial[0] with mask 2 and 32
    block[4] =
         (((initial[3] &  2)  ? 1 : 0) << 0) |
         (((initial[3] & 32)  ? 1 : 0) << 1) |
         (((initial[2] &  2)  ? 1 : 0) << 2) |
         (((initial[2] & 32)  ? 1 : 0) << 3) |
         (((initial[1] &  2)  ? 1 : 0) << 4) |
         (((initial[1] & 32)  ? 1 : 0) << 5) |
         (((initial[0] &  2)  ? 1 : 0) << 6) |
         (((initial[0] & 32)  ? 1 : 0) << 7);

    // Block[5]: using initial[7], initial[6], initial[5], initial[4] with mask 2 and 32
    block[5] =
         (((initial[7] &  2)  ? 1 : 0) << 0) |
         (((initial[7] & 32)  ? 1 : 0) << 1) |
         (((initial[6] &  2)  ? 1 : 0) << 2) |
         (((initial[6] & 32)  ? 1 : 0) << 3) |
         (((initial[5] &  2)  ? 1 : 0) << 4) |
         (((initial[5] & 32)  ? 1 : 0) << 5) |
         (((initial[4] &  2)  ? 1 : 0) << 6) |
         (((initial[4] & 32)  ? 1 : 0) << 7);

    // Block[6]: using initial[3], initial[2], initial[1], initial[0] with mask 1 and 16
    block[6] =
         (((initial[3] &  1)  ? 1 : 0) << 0) |
         (((initial[3] & 16)  ? 1 : 0) << 1) |
         (((initial[2] &  1)  ? 1 : 0) << 2) |
         (((initial[2] & 16)  ? 1 : 0) << 3) |
         (((initial[1] &  1)  ? 1 : 0) << 4) |
         (((initial[1] & 16)  ? 1 : 0) << 5) |
         (((initial[0] &  1)  ? 1 : 0) << 6) |
         (((initial[0] & 16)  ? 1 : 0) << 7);

    // Block[7]: using initial[7], initial[6], initial[5], initial[4] with mask 1 and 16
    block[7] =
         (((initial[7] &  1)  ? 1 : 0) << 0) |
         (((initial[7] & 16)  ? 1 : 0) << 1) |
         (((initial[6] &  1)  ? 1 : 0) << 2) |
         (((initial[6] & 16)  ? 1 : 0) << 3) |
         (((initial[5] &  1)  ? 1 : 0) << 4) |
         (((initial[5] & 16)  ? 1 : 0) << 5) |
         (((initial[4] &  1)  ? 1 : 0) << 6) |
         (((initial[4] & 16)  ? 1 : 0) << 7);
}

void pLayer(block_t *block)
{
    pLayer_hls(*block);
}

void present80_encryptBlock_hls(unsigned char block[PRESENT_BLOCK_SIZE_BYTES],
                                unsigned char key[PRESENT_80_KEY_SIZE_BYTES])
{
  keys_t roundKeys;
  unsigned char i;
  unsigned char j;
  generateRoundKeys80_hls(key, roundKeys);
  for (i = 0; i < (ROUNDS - 1); i++)
  {
    addRoundKey_hls(block, roundKeys[i]);
    // Unroll inner loop to reduce latency for the S-box substitution
    for (j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++) {
#pragma hls_unroll yes
      unsigned char temp = block[j];
      block[j] = (sBox[temp >> 4] << 4) | sBox[temp & 0xF];
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


