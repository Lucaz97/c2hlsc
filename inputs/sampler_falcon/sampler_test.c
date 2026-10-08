int main(void)
{
    sampler_context spc;
    fpr mu, isigma;
    int out;                     /* --- Initialize PRNG state (must be non-zero) --- */
    memset(&spc, 0, sizeof spc); /* Choose any non-zero 48-byte key/seed material */

    for (size_t i = 0; i < 48; i++)
    {
        spc.p.state.d[i] = (uint8_t)(0xA5u + (uint8_t)i);
    } /* 64-bit counter (ChaCha block counter) */

    *(uint64_t *)(spc.p.state.d + 48) = 1;             /* Fill buffer so prng_get_* works */
    prng_refill(&spc.p);                               /* sigma_min used by sampler */
    spc.sigma_min = fpr_of(12) / fpr_of(10); /* 1.2 */ /* --- Test inputs --- */
    mu = fpr_of(3) + fpr_of(1) / fpr_of(4);            /* 3.25 */
    isigma = fpr_of(1);                                /* 1/sigma (here sigma=1.0) */
    out = sampler(&spc, mu, isigma);
    printf("sampler(mu=3.25, isigma=1.0) -> %d\n", out);
    return 0;
}
