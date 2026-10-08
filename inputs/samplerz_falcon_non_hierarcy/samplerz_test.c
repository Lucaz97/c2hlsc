int main(void)
{
  uint8_t prng_buf[512];
  size_t prng_ptr = 0;
  uint8_t prng_state[256];
  fpr sigma_min;

  fpr mu, isigma;
  int out;

  memset(prng_buf, 0, sizeof prng_buf);
  memset(prng_state, 0, sizeof prng_state);

  for (size_t i = 0; i < 48; i++)
  {
    prng_state[i] = (uint8_t)(0xA5u + (uint8_t)i);
  }

  /* Catapult-safe: no (uint64_t*) cast */
  store_u64_le(prng_state + 48, 1);

  PQCLEAN_FALCONPADDED512_CLEAN_prng_refill(prng_buf, &prng_ptr, prng_state);

  sigma_min = fpr_of(12) / fpr_of(10); /* 1.2 */

  mu = fpr_of(3) + fpr_of(1) / fpr_of(4); /* 3.25 */
  isigma = fpr_of(1);

  out = sampler(prng_buf, &prng_ptr, prng_state, sigma_min, mu, isigma);
  printf("sampler(mu=3.25, isigma=1.0) -> %d\n", out);

  return 0;
}
