// ccs_block_macros.h
#include "ccs_testbench.h"

#ifndef EXCLUDE_CCS_BLOCK_INTERCEPT
#ifndef INCLUDE_CCS_BLOCK_INTERCEPT
#define INCLUDE_CCS_BLOCK_INTERCEPT
#ifdef  CCS_DESIGN_FUNC_MixColumns
#define ccs_intercept_MixColumns_20 \
  MixColumns(unsigned char state[4][4]);\
  extern void mc_testbench_capture_IN( unsigned char state[4][4] );\
  extern void mc_testbench_capture_OUT( unsigned char state[4][4] );\
  void ccs_real_MixColumns(unsigned char state[4][4]);\
  void MixColumns(unsigned char state[4][4])\
  {\
    static bool ccs_intercept_flag = false;\
    if (!ccs_intercept_flag) {\
      std::cout << "SCVerify intercepting C++ function 'MixColumns' for RTL block 'MixColumns'" << std::endl;\
      ccs_intercept_flag=true;\
    }\
    mc_testbench_wait_for_idle_sync();\
    mc_testbench_capture_IN(state);\
    ccs_real_MixColumns(state);\
    mc_testbench_capture_OUT(state);\
  }\
  void ccs_real_MixColumns
#else
#define ccs_intercept_MixColumns_20 MixColumns
#endif //CCS_DESIGN_FUNC_MixColumns
#endif //INCLUDE_CCS_BLOCK_INTERCEPT
#endif //EXCLUDE_CCS_BLOCK_INTERCEPT

// xtime 15 INLINE
#define ccs_intercept_xtime_15 xtime
// phex 35 INLINE
#define ccs_intercept_phex_35 phex
