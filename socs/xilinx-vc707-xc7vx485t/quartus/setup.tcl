project_new esp-quartus -family "Cyclone 10 GX" -overwrite
set_global_assignment -name VERILOG_INPUT_VERSION SYSTEMVERILOG_2012
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/vendor/lowrisc_ip/ip/prim/rtl}
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socgen/grlib} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socgen/esp}  
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/common/defs}
source ./srcs.tcl

