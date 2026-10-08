/*************************************************
 * Name:        shake256
 *
 * Description: SHAKE256 XOF with non-incremental API
 *
 * Arguments:   - unsigned char *output: pointer to output
 *              - unsigned long long outlen: requested output length in bytes
 *              - const unsigned char *input: pointer to input
 *              - unsigned long long inlen: length of input in bytes
 **************************************************/
void shake256(unsigned char *output,
              unsigned long long outlen,
              const unsigned char *input,
              unsigned long long inlen)
{
    unsigned int i;
    unsigned long nblocks = outlen / SHAKE256_RATE;
    unsigned char t[SHAKE256_RATE];
    uint64_t s[25];

    keccak_absorb(s, SHAKE256_RATE, input, inlen, 0x1F);
    keccak_squeezeblocks(output, nblocks, s, SHAKE256_RATE);

    output += nblocks * SHAKE256_RATE;
    outlen -= nblocks * SHAKE256_RATE;

    if (outlen)
    {
        keccak_squeezeblocks(t, 1, s, SHAKE256_RATE);
        for (i = 0; i < outlen; ++i)
            output[i] = t[i];
    }
}
