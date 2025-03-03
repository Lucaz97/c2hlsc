#! /bin/csh -f
setenv SYSTEMC_HOME /eda/mentor/Siemens_EDA/Catapult_Synthesis_2023.1_2/Mgc_home/shared
setenv SYSTEMC_LIB_DIR /eda/mentor/Siemens_EDA/Catapult_Synthesis_2023.1_2/Mgc_home/shared/lib
setenv CXX_FLAGS "-g -DCALYPTO_SKIP_SYSTEMC_VERSION_CHECK"
setenv LD_FLAGS "-lpthread"
setenv OSSIM ddd
setenv PATH /eda/mentor/Siemens_EDA/Catapult_Synthesis_2023.1_2/Mgc_home/bin:$PATH
