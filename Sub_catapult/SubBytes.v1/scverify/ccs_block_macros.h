// ccs_block_macros.h
#include "ccs_testbench.h"

#ifndef EXCLUDE_CCS_BLOCK_INTERCEPT
#ifndef INCLUDE_CCS_BLOCK_INTERCEPT
#define INCLUDE_CCS_BLOCK_INTERCEPT
#ifdef  CCS_DESIGN_FUNC_SubBytes
#define ccs_intercept_SubBytes_43 \
  SubBytes(unsigned char state[4][4]);\
  extern void mc_testbench_capture_IN( unsigned char state[4][4] );\
  extern void mc_testbench_capture_OUT( unsigned char state[4][4] );\
  void ccs_real_SubBytes(unsigned char state[4][4]);\
  void SubBytes(unsigned char state[4][4])\
  {\
    static bool ccs_intercept_flag = false;\
    if (!ccs_intercept_flag) {\
      std::cout << "SCVerify intercepting C++ function 'SubBytes' for RTL block 'SubBytes'" << std::endl;\
      ccs_intercept_flag=true;\
    }\
    mc_testbench_wait_for_idle_sync();\
    mc_testbench_capture_IN(state);\
    ccs_real_SubBytes(state);\
    mc_testbench_capture_OUT(state);\
  }\
  void ccs_real_SubBytes
#else
#define ccs_intercept_SubBytes_43 SubBytes
#endif //CCS_DESIGN_FUNC_SubBytes
#endif //INCLUDE_CCS_BLOCK_INTERCEPT
#endif //EXCLUDE_CCS_BLOCK_INTERCEPT

// phex 55 INLINE
#define ccs_intercept_phex_55 phex
