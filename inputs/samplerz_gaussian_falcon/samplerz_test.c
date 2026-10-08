static void fill_all(uint8_t *a, size_t n, uint8_t v)
{
  for (size_t i = 0; i < n; i++)
    a[i] = v;
}

static void fill_ramp(uint8_t *a, size_t n, uint8_t start)
{
  for (size_t i = 0; i < n; i++)
    a[i] = (uint8_t)(start + (uint8_t)i);
}

static void fill_alternating(uint8_t *a, size_t n, uint8_t a0, uint8_t a1)
{
  for (size_t i = 0; i < n; i++)
    a[i] = (i & 1) ? a1 : a0;
}

/* Simple xorshift32 for repeatable pseudo-random test buffers */
static uint32_t xorshift32(uint32_t *s)
{
  uint32_t x = *s;
  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  *s = x;
  return x;
}

static void fill_prng(uint8_t *a, size_t n, uint32_t seed)
{
  uint32_t s = seed ? seed : 1u;
  for (size_t i = 0; i < n; i++)
  {
    a[i] = (uint8_t)xorshift32(&s);
  }
}

static void init_state(uint8_t state_bytes[256], uint32_t seed)
{
  /* Just to vary state across tests (even if stub ignores it) */
  uint32_t s = seed ? seed : 0x12345678u;
  for (size_t i = 0; i < 256; i++)
  {
    state_bytes[i] = (uint8_t)(xorshift32(&s) ^ (uint32_t)i);
  }
}

/* Run one test case */
static void run_case(const char *name, uint8_t buf[512], uint8_t state_bytes[256], size_t ptr0)
{
  size_t ptr = ptr0 & 511;

  int z = sampler_gaussian(buf, &ptr, state_bytes);

  printf("[%-18s] ptr_in=%3zu  z=%4d  ptr_out=%3zu\n", name, ptr0 & 511, z, ptr);
}

int main(void)
{
  uint8_t buf[512];
  uint8_t state_bytes[256];

  /* 1) All zeros */
  fill_all(buf, sizeof buf, 0x00);
  init_state(state_bytes, 0x11111111u);
  run_case("all_zero_ptr0", buf, state_bytes, 0);

  /* 2) All 0xFF */
  fill_all(buf, sizeof buf, 0xFF);
  init_state(state_bytes, 0x22222222u);
  run_case("all_ff_ptr0", buf, state_bytes, 0);

  /* 3) Ramp starting at 0 */
  fill_ramp(buf, sizeof buf, 0x00);
  init_state(state_bytes, 0x33333333u);
  run_case("ramp0_ptr0", buf, state_bytes, 0);

  /* 4) Ramp starting at 0x80 */
  fill_ramp(buf, sizeof buf, 0x80);
  init_state(state_bytes, 0x44444444u);
  run_case("ramp80_ptr0", buf, state_bytes, 0);

  /* 5) Alternating 0xAA/0x55 */
  fill_alternating(buf, sizeof buf, 0xAA, 0x55);
  init_state(state_bytes, 0x55555555u);
  run_case("alt_AA55_ptr0", buf, state_bytes, 0);

  /* 6) Alternating 0x00/0xFF */
  fill_alternating(buf, sizeof buf, 0x00, 0xFF);
  init_state(state_bytes, 0x66666666u);
  run_case("alt_00FF_ptr0", buf, state_bytes, 0);

  /* 7) Pseudo-random buffer, seed 1 */
  fill_prng(buf, sizeof buf, 1u);
  init_state(state_bytes, 0x77777777u);
  run_case("prng1_ptr0", buf, state_bytes, 0);

  /* 8) Pseudo-random buffer, seed 0xDEADBEEF */
  fill_prng(buf, sizeof buf, 0xDEADBEEFu);
  init_state(state_bytes, 0x88888888u);
  run_case("prngDEAD_ptr0", buf, state_bytes, 0);

  /* 9) Wrap-around pointer test: start near end so u64+u8 wraps */
  fill_ramp(buf, sizeof buf, 0x00);
  init_state(state_bytes, 0x99999999u);
  run_case("ramp_ptr508", buf, state_bytes, 508);

  /* 10) Another wrap-around pointer test */
  fill_prng(buf, sizeof buf, 0xCAFEBABEu);
  init_state(state_bytes, 0xAAAAAAAAu);
  run_case("prng_ptr511", buf, state_bytes, 511);
  return 0;
}