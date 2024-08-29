# This Catapult LB script has been generated to expand the characterization range(es)
# of components of the Catapult base library(ies) to fit the current design
# 
# Running this script is optional but using the updated library should result in improved correlation.
# 
# Run this script in Catapult LB with the base library loaded or uncomment the "library load" command(s)
#library load /eda/mentor/Siemens_EDA/Catapult_Synthesis_2023.1_2/Mgc_home/pkgs/siflibs/nangate/nangate-45nm_beh.lib
library set /LIBS/nangate-45nm_beh/MODS/mgc_mux/PARAMETERS/ctrlw -- -CHAR_RANGE {1 to 5 by 1, 8}
library set /LIBS/nangate-45nm_beh/MODS/mgc_mux/PARAMETERS/p2ctrlw -- -CHAR_RANGE {2 to 2, 4 to 4, 8 to 8, 16 to 16, 32 to 32, 256}
# The "library characterize" command below requires that: 
#   1. characterization directory set in the library exists and write accessible;
#   2. paths to technology libraries are set to correct locations;
#   3. the downstream tool used to characterize the library is available;
library characterize
