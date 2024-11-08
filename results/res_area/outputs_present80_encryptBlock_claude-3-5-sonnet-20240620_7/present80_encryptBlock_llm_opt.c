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



#define PRESENT_80_KEY_SIZE_BYTES 10
#define PRESENT_BLOCK_SIZE_BYTES 8
#define ROUNDS               32
#define ROUND_KEY_SIZE_BYTES  8
typedef unsigned char keys_t[ROUNDS][ROUND_KEY_SIZE_BYTES];
typedef unsigned char present_key_t[PRESENT_80_KEY_SIZE_BYTES];
typedef unsigned char round_key_t[ROUND_KEY_SIZE_BYTES];
typedef unsigned char block_t[PRESENT_BLOCK_SIZE_BYTES];

// Note: sBox and sBoxInverse are assumed to be already defined

void copyKey(present_key_t from, present_key_t to)
{
    #pragma hls_pipeline_init_interval 1
    for (int i = 0; i < PRESENT_80_KEY_SIZE_BYTES; i++)
    {
        to[i] = from[i];
    }
}



#define PRESENT_80_KEY_SIZE_BYTES 10
#define PRESENT_BLOCK_SIZE_BYTES 8
#define ROUNDS               32
#define ROUND_KEY_SIZE_BYTES  8
typedef unsigned char keys_t[ROUNDS][ROUND_KEY_SIZE_BYTES];
typedef unsigned char present_key_t[PRESENT_80_KEY_SIZE_BYTES];
typedef unsigned char round_key_t[ROUND_KEY_SIZE_BYTES];
typedef unsigned char block_t[PRESENT_BLOCK_SIZE_BYTES];

void copyKey(present_key_t from, present_key_t to);

void copyBlock(block_t from, block_t to)
{
    #pragma HLS INLINE off
    for (int i = 0; i < PRESENT_BLOCK_SIZE_BYTES; i++)
    {
        #pragma HLS PIPELINE off
        to[i] = from[i];
    }
}



#define PRESENT_80_KEY_SIZE_BYTES 10
#define PRESENT_BLOCK_SIZE_BYTES 8
#define ROUNDS               32
#define ROUND_KEY_SIZE_BYTES  8
typedef unsigned char keys_t[ROUNDS][ROUND_KEY_SIZE_BYTES];
typedef unsigned char present_key_t[PRESENT_80_KEY_SIZE_BYTES];
typedef unsigned char round_key_t[ROUND_KEY_SIZE_BYTES];
typedef unsigned char block_t[PRESENT_BLOCK_SIZE_BYTES];

// Note: sBox and sBoxInverse are already defined in the provided code, so we don't need to redefine them here.

void copyKey(present_key_t from, present_key_t to);
void copyBlock(block_t from, block_t to);

void generateRoundKeys80(present_key_t suppliedKey, keys_t keys)
{
  present_key_t key;
  present_key_t newKey;
  unsigned char i, j;
  
  #pragma HLS ARRAY_PARTITION variable=key complete dim=1
  #pragma HLS ARRAY_PARTITION variable=newKey complete dim=1
  
  // Copy suppliedKey to key
  for (j = 0; j < PRESENT_80_KEY_SIZE_BYTES; j++) {
    #pragma HLS UNROLL
    key[j] = suppliedKey[j];
  }
  
  // Copy first round key
  for (j = 0; j < ROUND_KEY_SIZE_BYTES; j++) {
    #pragma HLS UNROLL
    keys[0][j] = key[j];
  }
  
  // Generate round keys
  for (i = 1; i < ROUNDS; i++)
  {
    #pragma HLS PIPELINE II=1
    
    // Generate new key
    for (j = 0; j < 10; j++)
    {
      #pragma HLS UNROLL
      newKey[j] = (key[(j + 7) % 10] << 5) | (key[(j + 8) % 10] >> 3);
    }

    // Copy new key to key
    for (j = 0; j < PRESENT_80_KEY_SIZE_BYTES; j++) {
      #pragma HLS UNROLL
      key[j] = newKey[j];
    }
    
    // Apply S-box and round constant
    key[0] = (sBox[key[0] >> 4] << 4) | (key[0] & 0xF);
    key[8] ^= i << 7;
    key[7] ^= i >> 1;
    
    // Copy round key
    for (j = 0; j < ROUND_KEY_SIZE_BYTES; j++) {
      #pragma HLS UNROLL
      keys[i][j] = key[j];
    }
  }
}



#define PRESENT_80_KEY_SIZE_BYTES 10
#define PRESENT_BLOCK_SIZE_BYTES 8
#define ROUNDS               32
#define ROUND_KEY_SIZE_BYTES  8
typedef unsigned char keys_t[ROUNDS][ROUND_KEY_SIZE_BYTES];
typedef unsigned char present_key_t[PRESENT_80_KEY_SIZE_BYTES];
typedef unsigned char round_key_t[ROUND_KEY_SIZE_BYTES];
typedef unsigned char block_t[PRESENT_BLOCK_SIZE_BYTES];

// These functions are assumed to be defined elsewhere
void copyKey(present_key_t from, present_key_t to);
void copyBlock(block_t from, block_t to);
void generateRoundKeys80(present_key_t suppliedKey, keys_t keys);

void addRoundKey(block_t block, round_key_t roundKey)
{
    #pragma HLS inline off
    unsigned char i;
    for (i = 0; i < 8; i++)
    {
        #pragma HLS pipeline off
        block[i] ^= roundKey[i];
    }
}



#define PRESENT_80_KEY_SIZE_BYTES 10
#define PRESENT_BLOCK_SIZE_BYTES 8
#define ROUNDS               32
#define ROUND_KEY_SIZE_BYTES  8
typedef unsigned char keys_t[ROUNDS][ROUND_KEY_SIZE_BYTES];
typedef unsigned char present_key_t[PRESENT_80_KEY_SIZE_BYTES];
typedef unsigned char round_key_t[ROUND_KEY_SIZE_BYTES];
typedef unsigned char block_t[PRESENT_BLOCK_SIZE_BYTES];

void copyBlock(block_t from, block_t to);

void pLayer(block_t block)
{
    #pragma HLS inline off
    block_t initial;
    copyBlock(block, initial);

    #pragma HLS array_partition variable=block complete
    #pragma HLS array_partition variable=initial complete

    unsigned char i, j;
    unsigned char indexVal, andVal;

    for (i = 0; i < 8; i++)
    {
        #pragma HLS pipeline off
        block[i] = 0;
        for (j = 0; j < 8; j++)
        {
            #pragma HLS unroll factor=2
            indexVal = (4 * (i & 1)) + (3 - (j >> 1));
            andVal = (8 >> (i >> 1)) << ((j & 1) << 2);
            block[i] |= ((initial[indexVal] & andVal) != 0) << j;
        }
    }
}



#define PRESENT_80_KEY_SIZE_BYTES 10
#define PRESENT_BLOCK_SIZE_BYTES 8
#define ROUNDS               32
#define ROUND_KEY_SIZE_BYTES  8
typedef unsigned char keys_t[ROUNDS][ROUND_KEY_SIZE_BYTES];
typedef unsigned char present_key_t[PRESENT_80_KEY_SIZE_BYTES];
typedef unsigned char round_key_t[ROUND_KEY_SIZE_BYTES];
typedef unsigned char block_t[PRESENT_BLOCK_SIZE_BYTES];

// Removed redefinitions of sBox and sBoxInverse

void copyKey(present_key_t from, present_key_t to);
void copyBlock(block_t from, block_t to);
void generateRoundKeys80(present_key_t suppliedKey, keys_t keys);
void addRoundKey(block_t block, round_key_t roundKey);
void pLayer(block_t block);

void present80_encryptBlock(block_t block, present_key_t key)
{
    #pragma HLS INLINE off
    keys_t roundKeys;
    unsigned char i;
    unsigned char j;
    
    generateRoundKeys80(key, roundKeys);
    
    #pragma HLS LOOP_MERGE
    #pragma HLS LOOP_FLATTEN
    for (i = 0; i < (ROUNDS - 1); i++)
    {
        #pragma HLS PIPELINE off
        addRoundKey(block, roundKeys[i]);
        
        #pragma HLS UNROLL factor=2
        for (j = 0; j < 8; j++)
        {
            block[j] = (sBox[block[j] >> 4] << 4) | sBox[block[j] & 0xF];
        }

        pLayer(block);
    }

    addRoundKey(block, roundKeys[ROUNDS - 1]);
}

int main(int argc, char *argv[])
{
  block_t block = {170, 187, 204, 221, 238, 255, 0, 17};
  present_key_t key = {1, 35, 69, 103, 137, 171, 205, 239, 1, 35};
  present80_encryptBlock(block, key);
  for (int _i = 0; _i < 8; _i++)
  {
    printf("%d ", block[_i]);
  }

  printf("\n");
  for (int _i = 0; _i < 10; _i++)
  {
    printf("%d ", key[_i]);
  }

  printf("\n");
  return 0;
}
