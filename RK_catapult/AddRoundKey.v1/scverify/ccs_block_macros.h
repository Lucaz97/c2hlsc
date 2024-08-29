// ccs_block_macros.h
#include "ccs_testbench.h"

#ifndef EXCLUDE_CCS_BLOCK_INTERCEPT
#ifndef INCLUDE_CCS_BLOCK_INTERCEPT
#define INCLUDE_CCS_BLOCK_INTERCEPT
#ifdef  CCS_DESIGN_FUNC_AddRoundKey
#define ccs_intercept_AddRoundKey_14 \
  AddRoundKey(unsigned char round, unsigned char state[4][4], const unsigned char *RoundKey);\
  extern void mc_testbench_capture_IN( unsigned char round, unsigned char state[4][4], const unsigned char *RoundKey );\
  extern void mc_testbench_capture_OUT( unsigned char round, unsigned char state[4][4], const unsigned char *RoundKey );\
  void ccs_real_AddRoundKey(unsigned char round, unsigned char state[4][4], const unsigned char *RoundKey);\
  void AddRoundKey(unsigned char round, unsigned char state[4][4], const unsigned char *RoundKey)\
  {\
    static bool ccs_intercept_flag = false;\
    if (!ccs_intercept_flag) {\
      std::cout << "SCVerify intercepting C++ function 'AddRoundKey' for RTL block 'AddRoundKey'" << std::endl;\
      ccs_intercept_flag=true;\
    }\
    mc_testbench_wait_for_idle_sync();\
    mc_testbench_capture_IN(round, state, RoundKey);\
    ccs_real_AddRoundKey(round, state, RoundKey);\
    mc_testbench_capture_OUT(round, state, RoundKey);\
  }\
  void ccs_real_AddRoundKey
#else
#define ccs_intercept_AddRoundKey_14 AddRoundKey
#endif //CCS_DESIGN_FUNC_AddRoundKey
#endif //INCLUDE_CCS_BLOCK_INTERCEPT
#endif //EXCLUDE_CCS_BLOCK_INTERCEPT

// phex 34 INLINE
#define ccs_intercept_phex_34 phex
