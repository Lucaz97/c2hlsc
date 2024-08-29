#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define Nb 4
#define Nk 4        // The number of 32 bit words in a key.
#define Nr 10       // The number of rounds in AES Cipher.

 #define AES_KEYLEN 16   // Key length in bytes
    #define AES_keyExpSize 176
typedef uint8_t state_t[4][4];

// This function adds the round key to state.
// The round key is added to the state by an XOR function.
void AddRoundKey(uint8_t round, uint8_t state[4][4], const uint8_t* RoundKey)
{
    uint8_t i, j;
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            state[i][j] ^= RoundKey[(round * Nb * 4) + (i * Nb) + j];
        }
    }
}