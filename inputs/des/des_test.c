/*********************************************************************
* Filename:   des_test.c
* Author:     Brad Conte (brad AT bradconte.com)
* Copyright:
* Disclaimer: This code is presented "as is" without any guarantees.
* Details:    Performs known-answer tests on the corresponding DES
	          implementation. These tests do not encompass the full
	          range of available test vectors, however, if the tests
	          pass it is very, very likely that the code is correct
	          and was compiled properly. This code also serves as
	          example usage of the functions.
*********************************************************************/

/*************************** HEADER FILES ***************************/
#include <stdio.h>
#include <memory.h>


int main()
{
	des_block_t pt1 = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xE7};
	unsigned char key1[DES_BLOCK_SIZE] = {0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};
	
	des_key_t schedule;
	des_block_t buf;
	
	des_key_setup(key1, schedule, DES_ENCRYPT);
	des_crypt(&pt1, &buf, &schedule);

	for (int i = 0; i < 8; i++)
		printf("%02X ", buf[i]);

	return 0;

}
