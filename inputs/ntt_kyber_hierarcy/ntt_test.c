static void print_poly(const char *label, const int16_t r[256])
{
  printf("%s:\n", label);
  for (int i = 0; i < 5; i++)
  { // print first 16 entries only
    printf("%6d ", r[i]);
  }
  printf("\n...\n\n");
}

static uint32_t xorshift32(uint32_t *state)
{
  uint32_t x = *state;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *state = x;
  return x;
}

int main(void)
{
  enum
  {
    NUM_TESTS = 1000
  };

  uint32_t rng = 0xC0FFEE01u; // deterministic seed
  int failures = 0;

  for (int tc = 0; tc < NUM_TESTS; tc++)
  {
    int16_t r[N];
    int16_t in[N];

    /* -------------------------------------------------
     * Generate random test polynomial
     * ------------------------------------------------- */
    for (int i = 0; i < N; i++)
    {
      r[i] = (int16_t)(xorshift32(&rng) % Q);
    }

    memcpy(in, r, sizeof(r));

    /* Print a few example cases */
    if (tc < 3)
    {
      print_poly("Input polynomial", r);
    }

    /* -------------------------------------------------
     * Run NTT
     * ------------------------------------------------- */
    ntt(r);

    if (tc < 3)
    {
      print_poly("After NTT", r);
    }

    /* -------------------------------------------------
     * Sanity checks
     * ------------------------------------------------- */
    int all_zero = 1;
    for (int i = 0; i < N; i++)
    {
      if (r[i] != 0)
      {
        all_zero = 0;
        break;
      }
    }

    if (all_zero)
    {
      failures++;
      printf("[FAIL] Test %d produced all-zero output\n", tc);
      print_poly("Failing input (preview)", in);
    }
  }

  /* -------------------------------------------------
   * Summary
   * ------------------------------------------------- */
  if (failures == 0)
  {
    printf("PASS: %d/%d NTT tests completed successfully.\n",
           NUM_TESTS, NUM_TESTS);
    return 0;
  }
  else
  {
    printf("DONE: %d failures out of %d tests.\n",
           failures, NUM_TESTS);
    return 1;
  }
}
