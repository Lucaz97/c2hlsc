// ccs_block_macros.h
#include "ccs_testbench.h"

#ifndef EXCLUDE_CCS_BLOCK_INTERCEPT
#ifndef INCLUDE_CCS_BLOCK_INTERCEPT
#define INCLUDE_CCS_BLOCK_INTERCEPT
#ifdef  CCS_DESIGN_FUNC_filter
#define ccs_intercept_filter_116 \
  filter(int input, int *fir_output, int *iir_output);\
  extern void mc_testbench_capture_IN( int input, int *fir_output, int *iir_output );\
  extern void mc_testbench_capture_OUT( int input, int *fir_output, int *iir_output );\
  void ccs_real_filter(int input, int *fir_output, int *iir_output);\
  void filter(int input, int *fir_output, int *iir_output)\
  {\
    static bool ccs_intercept_flag = false;\
    if (!ccs_intercept_flag) {\
      std::cout << "SCVerify intercepting C++ function 'filter' for RTL block 'filter'" << std::endl;\
      ccs_intercept_flag=true;\
    }\
    mc_testbench_wait_for_idle_sync();\
    mc_testbench_capture_IN(input, fir_output, iir_output);\
    ccs_real_filter(input, fir_output, iir_output);\
    mc_testbench_capture_OUT(input, fir_output, iir_output);\
  }\
  void ccs_real_filter
#else
#define ccs_intercept_filter_116 filter
#endif //CCS_DESIGN_FUNC_filter
#endif //INCLUDE_CCS_BLOCK_INTERCEPT
#endif //EXCLUDE_CCS_BLOCK_INTERCEPT

// shift 17 INLINE
#define ccs_intercept_shift_17 shift
// mac 33 INLINE
#define ccs_intercept_mac_33 mac
// iir 45 INLINE
#define ccs_intercept_iir_45 iir
// fir 104 INLINE
#define ccs_intercept_fir_104 fir
