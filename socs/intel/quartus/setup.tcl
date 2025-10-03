project_new top -family "Cyclone 10 GX" -overwrite
#project_new top -family "Stratix 10" -device "1SG280HU2F50E2VG" -overwrite
set_global_assignment -name VERILOG_INPUT_VERSION SYSTEMVERILOG_2009
package require ::quartus::project
set_global_assignment -name BOARD "Intel Stratix 10 GX FPGA Development Kit DK-DEV-1SGX-H-A"
set_global_assignment -name IP_FILE emif.ip

 #quartus_ipgenerate -synthesis vhdl -simulation vhdl -simulator modelsim -clear_ip_generation_dirs

## Ibex
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/vendor/lowrisc_ip/ip/prim/rtl}
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel/socgen/grlib} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel/socgen/esp}  
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/common/defs}
#source ./srcs.tcl

## Ariane
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/cores/ariane/ariane/src/common_cells/include}
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel/socgen/grlib} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel/socgen/esp}  
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/common/defs}
source ./srcs_ariane.tcl

## Leon3
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel/socgen/grlib} 
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/socs/intel/socgen/esp}  
set_global_assignment -name USER_LIBRARIES {/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/common/defs}
#source ./srcs_leon3.tcl

# simulation generation 
# quartus_sh --simlib_comp -tool questasim -language vhdl -tool_path /opt/mentor/modeltech/bin/ -directory /home/yaagna/intel_esp/esp/socs/intel/quartus/sim -rtl_only


