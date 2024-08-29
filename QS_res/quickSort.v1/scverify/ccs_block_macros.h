// ccs_block_macros.h
#include "ccs_testbench.h"

#ifndef EXCLUDE_CCS_BLOCK_INTERCEPT
#ifndef INCLUDE_CCS_BLOCK_INTERCEPT
#define INCLUDE_CCS_BLOCK_INTERCEPT
#ifdef  CCS_DESIGN_FUNC_quickSort
#define ccs_intercept_quickSort_42 \
  quickSort(int arr[64], int low, int high);\
  extern void mc_testbench_capture_IN( int arr[64], int low, int high );\
  extern void mc_testbench_capture_OUT( int arr[64], int low, int high );\
  void ccs_real_quickSort(int arr[64], int low, int high);\
  void quickSort(int arr[64], int low, int high)\
  {\
    static bool ccs_intercept_flag = false;\
    if (!ccs_intercept_flag) {\
      std::cout << "SCVerify intercepting C++ function 'quickSort' for RTL block 'quickSort'" << std::endl;\
      ccs_intercept_flag=true;\
    }\
    mc_testbench_wait_for_idle_sync();\
    mc_testbench_capture_IN(arr, low, high);\
    ccs_real_quickSort(arr, low, high);\
    mc_testbench_capture_OUT(arr, low, high);\
  }\
  void ccs_real_quickSort
#else
#define ccs_intercept_quickSort_42 quickSort
#endif //CCS_DESIGN_FUNC_quickSort
#endif //INCLUDE_CCS_BLOCK_INTERCEPT
#endif //EXCLUDE_CCS_BLOCK_INTERCEPT

// swap 5 INLINE
#define ccs_intercept_swap_5 swap
// partition 13 INLINE
#define ccs_intercept_partition_13 partition
