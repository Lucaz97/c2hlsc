// mc_dut_wrapper.h
#ifndef INCLUDED_CCS_DUT_WRAPPER_H
#define INCLUDED_CCS_DUT_WRAPPER_H

#ifndef SC_USE_STD_STRING
#define SC_USE_STD_STRING
#endif

#include <systemc.h>
#include <mc_simulator_extensions.h>

#ifdef CCS_SYSC
namespace HDL {
#endif
#if defined(CCS_DUT_SYSC)
// alias ccs_DUT_wrapper to namespace enclosure of either cycle or RTL SystemC netlist
namespace
    ccs_design {
#if defined(CCS_DUT_CYCLE)
#include "cycle.cxx"
#else
#if defined(CCS_DUT_RTL)
#include "rtl.cxx"
#endif
#endif
}
typedef
    ccs_design::HDL::quickSort ccs_DUT_wrapper;

#else

// Create a foreign module wrapper around the HDL
#ifdef VCS_SYSTEMC
// VCS support - ccs_DUT_wrapper is derived from VCS-generated SystemC wrapper around HDL code
class ccs_DUT_wrapper : public TOP_HDL_ENTITY
{
public:
  ccs_DUT_wrapper(const sc_module_name& nm, const char *hdl_name)
  : TOP_HDL_ENTITY(nm)
  {
  // elaborate_foreign_module(hdl_name);
  }

  ~ccs_DUT_wrapper() {}
};

#else
// non VCS simulators - ccs_DUT_wrapper is derived from mc_foreign_module (adding 2nd ctor arg)
class ccs_DUT_wrapper: public mc_foreign_module
{
public:
  // Interface Ports
  sc_in<bool> clk;
  sc_in<sc_logic> rst_n;
  sc_out<sc_lv<2048> > arr_rsc_zout;
  sc_out<sc_logic> arr_rsc_lzout;
  sc_in<sc_lv<2048> > arr_rsc_zin;
  sc_out<sc_logic> arr_triosy_lz;
  sc_in<sc_lv<32> > low_rsc_dat;
  sc_out<sc_logic> low_triosy_lz;
  sc_in<sc_lv<32> > high_rsc_dat;
  sc_out<sc_logic> high_triosy_lz;
public:
  ccs_DUT_wrapper(const sc_module_name& nm, const char *hdl_name)
  :
    mc_foreign_module(nm, hdl_name), 
    clk("clk"), 
    rst_n("rst_n"), 
    arr_rsc_zout("arr_rsc_zout"), 
    arr_rsc_lzout("arr_rsc_lzout"), 
    arr_rsc_zin("arr_rsc_zin"), 
    arr_triosy_lz("arr_triosy_lz"), 
    low_rsc_dat("low_rsc_dat"), 
    low_triosy_lz("low_triosy_lz"), 
    high_rsc_dat("high_rsc_dat"), 
    high_triosy_lz("high_triosy_lz")
  {
    elaborate_foreign_module(hdl_name);
  }

  ~ccs_DUT_wrapper() {}
};
#endif

#endif

#ifdef CCS_SYSC
} // end namespace HDL
#endif
#endif


