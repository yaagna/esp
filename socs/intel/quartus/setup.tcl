project_new top -family "Cyclone 10 GX" -overwrite
#set_global_assignment -name VERILOG_INPUT_VERSION SYSTEMVERILOG_2009
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/vendor/lowrisc_ip/ip/prim/rtl}
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel/socgen/grlib} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel/socgen/esp}  
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/common/defs}
source ./srcs.tcl

