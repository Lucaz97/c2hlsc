int main(void)
{
    uint64_t mu, isigma;
    int out;
    size_t i;

    memset(global_state, 0, sizeof global_state);
    for (i = 0; i < 48; i++)
    {
        global_state[i] = (uint8_t)(0xA5u + (uint8_t)i);
    }
    *(uint64_t *)(global_state + 48) = 1;

    prng_refill();
    global_sigma_min = fpr_of(12) / fpr_of(10);
    mu = fpr_of(3) + fpr_of(1) / fpr_of(4);
    isigma = fpr_of(1);
    out = sampler(mu, isigma);
    printf("sampler(mu=3.25, isigma=1.0) -> %d\n", out);
    return 0;
}
