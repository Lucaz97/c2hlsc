void mc_testbench_capture_IN( unsigned char round, unsigned char state[4][4], const unsigned char *RoundKey ) {
  mc_testbench::capture_IN(round, state, RoundKey);
}
void mc_testbench_capture_OUT( unsigned char round, unsigned char state[4][4], const unsigned char *RoundKey ) {
  mc_testbench::capture_OUT(round, state, RoundKey);
}
