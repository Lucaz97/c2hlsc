#ifndef ASCON_PERM_H
#define ASCON_PERM_H

#include <stdint.h>

#define N_ROUNDS_A 12
#define N_ROUNDS_B 6


void permute_a(uint64_t *state);
void permute_b(uint64_t *state);


#endif
