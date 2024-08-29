// ccs_block_macros.h
#include "ccs_testbench.h"

#ifndef EXCLUDE_CCS_BLOCK_INTERCEPT
#ifndef INCLUDE_CCS_BLOCK_INTERCEPT
#define INCLUDE_CCS_BLOCK_INTERCEPT
#ifdef  CCS_DESIGN_FUNC_ShiftRows
#define ccs_intercept_ShiftRows_19 \
  ShiftRows(unsigned char state[4][4]);\
  extern void mc_testbench_capture_IN( unsigned char state[4][4] );\
  extern void mc_testbench_capture_OUT( unsigned char state[4][4] );\
  void ccs_real_ShiftRows(unsigned char state[4][4]);\
  void ShiftRows(unsigned char state[4][4])\
  {\
    static bool ccs_intercept_flag = false;\
    if (!ccs_intercept_flag) {\
      std::cout << "SCVerify intercepting C++ function 'ShiftRows' for RTL block 'ShiftRows'" << std::endl;\
      ccs_intercept_flag=true;\
    }\
    mc_testbench_wait_for_idle_sync();\
    mc_testbench_capture_IN(state);\
    ccs_real_ShiftRows(state);\
    mc_testbench_capture_OUT(state);\
  }\
  void ccs_real_ShiftRows
#else
#define ccs_intercept_ShiftRows_19 ShiftRows
#endif //CCS_DESIGN_FUNC_ShiftRows
#endif //INCLUDE_CCS_BLOCK_INTERCEPT
#endif //EXCLUDE_CCS_BLOCK_INTERCEPT

// phex 46 INLINE
#define ccs_intercept_phex_46 phex
