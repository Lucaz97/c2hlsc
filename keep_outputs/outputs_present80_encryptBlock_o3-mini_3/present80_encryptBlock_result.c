
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
    #pragma HLS inline
    uint64_t data64 = 0;
    uint16_t data16 = 0;
    int i;

    // Pack the first 8 bytes into a 64-bit word.
    for (i = 0; i < 8; i++) {
        data64 |= ((uint64_t)from[i]) << (8 * i);
    }

    // Pack the remaining 2 bytes into a 16-bit word.
    for (i = 0; i < 2; i++) {
        data16 |= ((uint16_t)from[8 + i]) << (8 * i);
    }

    // Unpack the 64-bit word into 8 bytes of the destination.
    for (i = 0; i < 8; i++) {
        to[i] = (unsigned char)(data64 >> (8 * i));
    }

    // Unpack the 16-bit word into the remaining 2 bytes of the destination.
    for (i = 0; i < 2; i++) {
        to[8 + i] = (unsigned char)(data16 >> (8 * i));
    }
}

void copyKey(present_key_t *from, present_key_t *to)
{
    copyKey_hls(*from, *to);
}

void copyBlock_hls(unsigned char from[PRESENT_BLOCK_SIZE_BYTES],
                   unsigned char to[PRESENT_BLOCK_SIZE_BYTES])
{
    #pragma HLS inline

    uint64_t temp = 0;
    for (int i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++) {
        temp |= ((uint64_t)from[i]) << (8 * i);
    }
    for (int i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++) {
        to[i] = (unsigned char)((temp >> (8 * i)) & 0xFF);
    }
}

void copyBlock(block_t *from, block_t *to)
{
    copyBlock_hls(*from, *to);
}

#define PRESENT_80_KEY_SIZE_BYTES 10
#define PRESENT_BLOCK_SIZE_BYTES 8
#define ROUNDS               32
#define ROUND_KEY_SIZE_BYTES  8

typedef unsigned char keys_t[ROUNDS][ROUND_KEY_SIZE_BYTES];
typedef unsigned char present_key_t[PRESENT_80_KEY_SIZE_BYTES];
typedef unsigned char round_key_t[ROUND_KEY_SIZE_BYTES];
typedef unsigned char block_t[PRESENT_BLOCK_SIZE_BYTES];

// Assume these are defined elsewhere
extern unsigned char sBox[16];
void copyKey_hls(unsigned char from[PRESENT_80_KEY_SIZE_BYTES],
                 unsigned char to[PRESENT_80_KEY_SIZE_BYTES]);
void copyBlock_hls(unsigned char from[PRESENT_BLOCK_SIZE_BYTES],
                   unsigned char to[PRESENT_BLOCK_SIZE_BYTES]);

// Optimized for latency
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
#pragma hls_pipeline_init_interval 0
        for (j = 0; j < PRESENT_80_KEY_SIZE_BYTES; j++)
        {
#pragma hls_unroll yes
            newKey[j] = (key[(j + 7) % PRESENT_80_KEY_SIZE_BYTES] << 5) |
                        (key[(j + 8) % PRESENT_80_KEY_SIZE_BYTES] >> 3);
        }

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
    // Use ac_int<64,false> to represent a 64-bit unsigned integer.
    ac_int<64, false> block_word = 0;
    ac_int<64, false> key_word   = 0;
    
    // Fully unroll the pack loops to create combinational logic for packing
    for (int i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++) {
        #pragma hls_unroll yes
        block_word |= ((ac_int<64, false>)block[i]) << (8 * i);
        key_word   |= ((ac_int<64, false>)roundKey[i]) << (8 * i);
    }
    
    // Perform the XOR on the full 64-bit word in one operation.
    block_word ^= key_word;
    
    // Unpack the result back into the 8-byte block.
    for (int i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++) {
        #pragma hls_unroll yes
        block[i] = (unsigned char)(block_word >> (8 * i));
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

    // Fully unroll outer loop to minimize latency
    for (i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++) {
    #pragma hls_unroll yes
        block[i] = 0;
        // Fully unroll inner loop to further reduce latency
        for (j = 0; j < PRESENT_BLOCK_SIZE_BYTES; j++) {
        #pragma hls_unroll yes
            indexVal = (4 * (i % 2)) + (3 - (j >> 1));
            andVal   = (8 >> (i >> 1)) << ((j % 2) << 2);
            block[i] |= ((initial[indexVal] & andVal) != 0) << j;
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


