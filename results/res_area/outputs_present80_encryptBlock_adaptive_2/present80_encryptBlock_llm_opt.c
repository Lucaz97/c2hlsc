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

void copyKey(present_key_t from, present_key_t to)
{
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
    int i;
    for (i = 0; i < 8; i++)
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
void copyBlock(block_t from, block_t to);

void generateRoundKeys80(present_key_t suppliedKey, keys_t keys)
{
    present_key_t key;
    present_key_t newKey;
    unsigned char i;
    unsigned char j;
    
    // Copy the supplied key to the local key variable
    copyKey(suppliedKey, key);
    
    // Copy the first key block to keys
    copyBlock(key, keys[0]); // Pass the first 8 bytes of key as a block_t
    
    for (i = 1; i < 32; i++)
    {
        for (j = 0; j < 10; j++)
        {
            newKey[j] = (key[(j + 7) % 10] << 5) | (key[(j + 8) % 10] >> 3);
        }

        copyKey(newKey, key);
        key[0] = (sBox[key[0] >> 4] << 4) | (key[0] & 0xF);
        key[8] ^= i << 7;
        key[7] ^= i >> 1;
        
        // Copy the current key block to keys
        copyBlock(key, keys[i]); // Pass the first 8 bytes of key as a block_t
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
void copyBlock(block_t from, block_t to);
void generateRoundKeys80(present_key_t suppliedKey, keys_t keys);

void addRoundKey(block_t block, round_key_t roundKey)
{
  unsigned char i;
  for (i = 0; i < 8; i++)
  {
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

void copyKey(present_key_t from, present_key_t to);
void copyBlock(block_t from, block_t to);
void generateRoundKeys80(present_key_t suppliedKey, keys_t keys);
void addRoundKey(block_t block, round_key_t roundKey);

void pLayer(block_t block)
{
  unsigned char i;
  unsigned char j;
  unsigned char indexVal;
  unsigned char andVal;
  block_t initial;
  copyBlock(block, initial);
  for (i = 0; i < 8; i++)
  {
    block[i] = 0;
    for (j = 0; j < 8; j++)
    {
      indexVal = (4 * (i % 2)) + (3 - (j >> 1));
      andVal = (8 >> (i >> 1)) << ((j % 2) << 2);
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

void copyKey(present_key_t from, present_key_t to);
void copyBlock(block_t from, block_t to);
void generateRoundKeys80(present_key_t suppliedKey, keys_t keys);
void addRoundKey(block_t block, round_key_t roundKey);
void pLayer(block_t block);

void present80_encryptBlock(block_t block, present_key_t key)
{
  keys_t roundKeys;
  unsigned char i;
  unsigned char j;
  generateRoundKeys80(key, roundKeys);
  for (i = 0; i < (32 - 1); i++)
  {
    addRoundKey(block, roundKeys[i]);
    for (j = 0; j < 8; j++)
    {
      block[j] = (sBox[block[j] >> 4] << 4) | sBox[block[j] & 0xF];
    }

    pLayer(block);
  }

  addRoundKey(block, roundKeys[32 - 1]);
}

int main()
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
}
