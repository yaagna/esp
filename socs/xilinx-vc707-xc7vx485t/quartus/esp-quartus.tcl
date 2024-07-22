# Copyright (C) 2021  Intel Corporation. All rights reserved.
# Your use of Intel Corporation's design tools, logic functions 
# and other software and tools, and any partner logic 
# functions, and any output files from any of the foregoing 
# (including device programming or simulation files), and any 
# associated documentation or information are expressly subject 
# to the terms and conditions of the Intel Program License 
# Subscription Agreement, the Intel Quartus Prime License Agreement,
# the Intel FPGA IP License Agreement, or other applicable license
# agreement, including, without limitation, that your use is for
# the sole purpose of programming logic devices manufactured by
# Intel and sold by Intel or its authorized distributors.  Please
# refer to the applicable agreement for further details, at
# https://fpgasoftware.intel.com/eula.

# Quartus Prime: Generate Tcl File for Project
# File: esp-quartus.tcl
# Generated on: Thu Jun 27 12:26:19 2024

# Load Quartus Prime Tcl Project package
package require ::quartus::project

set need_to_close_project 0
set make_assignments 1

# Check that the right project is open
if {[is_project_open]} {
	if {[string compare $quartus(project) "esp-quartus"]} {
		puts "Project esp-quartus is not open"
		set make_assignments 0
	}
} else {
	# Only open if not already open
	if {[project_exists esp-quartus]} {
		project_open -revision esp-quartus esp-quartus
	} else {
		project_new -revision esp-quartus esp-quartus
	}
	set need_to_close_project 1
}

# Make assignments
if {$make_assignments} {
	set_global_assignment -name ORIGINAL_QUARTUS_VERSION 21.1.0
	set_global_assignment -name PROJECT_CREATION_TIME_DATE "12:22:46  JUNE 27, 2024"
	set_global_assignment -name LAST_QUARTUS_VERSION "21.1.0 Pro Edition"
	set_global_assignment -name DEVICE 10CX220YF780I5G
	set_global_assignment -name SEARCH_PATH /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/vendor/lowrisc_ip/ip/prim/rtl/
	set_global_assignment -name SEARCH_PATH "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/"
	set_global_assignment -name SEARCH_PATH "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socgen/grlib/"
	set_global_assignment -name SEARCH_PATH "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socgen/esp/"
	set_global_assignment -name SEARCH_PATH "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/common/defs/"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socgen/esp/esp_global.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socketgen/sld_devices.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socketgen/allacc.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socketgen/genacc.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socketgen/allcaches.vhd"
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/stdlib/version.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/stdlib/config_types.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/stdlib/config.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/stdlib/stdlib.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/gencomp.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/allpads.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/allclkgen.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/alldco.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/allddr.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/allmem.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/allpll.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/allmul.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/bus/amba.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/bus/devices.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/sparc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/misc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sim/utils/stdio.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sim/utils/testlib.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sim/utils/sim.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sim/utils/sparc_disas.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sim/utils/cpu_disas.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sim/tb/tb_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/multlib.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/coretypes.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/sldfpu/basic.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/sldfpu/bw.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/sldfpu/itsqrdiv.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/sldfpu/sldfpp.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/arith.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/leon3.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/mmuconfig.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/mmuiface.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/libmmu.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/libfpu.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/libiu.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/libcache.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/leon3/libleon3.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex_esp_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/ariane/ariane_esp_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/esp_acc_regmap.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/monitor/monitor_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/csr/esp_csr_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/genram_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/nocpackage.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/caches/cachepackage.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/caches/gencaches.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/dvfs/dvfs.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/tile.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/jtag_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/ethcomp.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/uart/uart.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/uart/libdcom.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/video/svga_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/timer/gptimer_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ddr/ahb2mig_7series_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/net.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/ethernet_mac.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/greth_pkg.vhd
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socgen/grlib/grlib_config.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socgen/esp/socmap.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socketgen/sldacc.vhd"
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/tiles_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/asic/tiles_asic_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/fpga/tiles_fpga_pkg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/syncrambw.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/outpad_ds.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/pll.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/iopad_tm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/iopad_ds.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/memrwcol.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/ddr_ireg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/syncram64.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/clkmux.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/syncram_dp.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/inpad_ds.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/dco.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/syncram_2p.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/clkgen.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/iopad_ddr.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/inpad.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/outpad_ddr.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/outpad.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/clkpad.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/syncreg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/ddr_oreg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/clkand.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/syncram.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/iopad.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/inpad_ddr.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/clkpad_ds.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/regfile_3p.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/techmult.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/maps/grgates.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/inferred/mul_inferred.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/inferred/ddr_inferred.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/inferred/memory_inferred.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/techmap/inferred/fifo_inferred.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/greth_rx.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/greth_tx.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/eth_ahb_mst.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/grethm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/greth.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/grethc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/eth_edcl_ahb_mst.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ethernet/eth_rstgen.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/timer/gptimer.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/uart/dcom.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/uart/apbuart.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/video/svgactrl.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/video/svga2tfp410.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/interrupt/irqmp.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ddr/ahb2mig_7series.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ddr/ahb2mig_7series_profpga.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ddr/ahb2mig_up.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ddr/ahb2mig_ebddr4r5.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/peripherals/ddr/ahb2bsg_dmc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/caches/l2_wrapper.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/caches/llc_wrapper.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/caches/l2_acc_wrapper.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/caches/fifo_custom.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/ahbram.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/unread.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/ahbslm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/esp_init.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/ahbram_dp.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/misc/rstgen.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/sync_noc_set.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/noc32_xy.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/fifo.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/fifo2.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/sync_noc_xy.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/inferred_async_fifo.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/fifo3.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/fifo1.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/sync_noc32_xy.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/noc/router.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/noc2apb.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/noc2intreq.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/intreq2noc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/ahbslv2noc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/gp_arbiter.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/esp_acc_dma.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/apb2noc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/ext2ahbm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/iolink2ahbm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/ahbslv2iolink.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/esp_acc_tlb.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/noc2ahbmst.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/fixen_64to32.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/intack2noc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/mem2ext.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/proxy/axislv2noc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/adapters/ahb2axi_l.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/queues/slm_tile_q.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/queues/acc_tile_q.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/queues/cpu_tile_q.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/queues/mem_tile_q.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/queues/empty_tile_q.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/queues/misc_tile_q.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/csr/esp_tile_csr.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/monitor/monitor.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/mux_6to1.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/demux_1to6.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/demux_1to2.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/demux_1to6_vs.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/counter_jtag.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/jtag_test.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/piso_jtag.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/sipo_jtag.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/jtag2apb.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/apb2jtag.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/jtag2apb_reg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/apb2jtag_reg.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/jtag_apb_config.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/jtag_apb_slv_config.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/jtag_apb_slv.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/fpga_proxy_jtag.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/jtag/jtag_tb.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/dvfs/dvfs_top.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/dvfs/dvfs_fsm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/bus/patient_apbctrl.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/bus/ahbmst.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/bus/ahbtrace_mb.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/bus/ahbtrace_mmb.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/bus/ahbtrace.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/sockets/bus/ahbctrl.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/tile_empty.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/tile_slm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/tile_mem.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/tile_cpu.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/tile_io.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/asic/asic_tile_acc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/asic/asic_tile_cpu.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/asic/asic_tile_empty.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/asic/asic_tile_io.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/asic/asic_tile_mem.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/asic/asic_tile_slm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/asic/asic_tile_slm_ddr.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/fpga/fpga_tile_acc.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/fpga/fpga_tile_cpu.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/fpga/fpga_tile_empty.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/fpga/fpga_tile_io.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/fpga/fpga_tile_mem.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/fpga/fpga_tile_slm.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/tiles/esp.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex_ahb_wrap.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex_timer_apb_wrap.vhd
	set_global_assignment -name VHDL_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/riscv_plic_apb_wrap.vhd
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socketgen/accelerators.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socketgen/caches.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socketgen/tile_acc.vhd"
	set_global_assignment -name VHDL_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/top.vhd"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_bufs.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_regs.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_interfaces.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_interface_defs.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_lookup_way.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/pri_enc_half.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_localmem.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_localmem_asic.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_update.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_rtl_top.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/pri_enc.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_process_request.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_core.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/llc/rtl/llc_input_decoder.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_fsm.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_bufs.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_interface_defs.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_write_word.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_rtl_top.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_interfaces.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_lookup.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_localmem.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_localmem_asic.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_regs.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_reqs.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_core.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/l2/rtl/l2_input_decoder.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/rtl/caches/esp-caches/common/rtl/interface_controller.sv"
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/misc/esplink.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/sockets/adapters/apb2axil.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/noc/router/noc_pkg.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/noc/router/router_fifo.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/noc/router/router_arbiter.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/noc/router/lookahead_routing.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/noc/router/lookahead_router.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/noc/router/lookahead_router_wrapper.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_pkg.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex_wrap.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex_timer_wrap.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/reg_intf_pkg.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/reg_intf.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/apb_to_reg.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/riscv_plic_wrap.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/rv_plic/rv_plic_target.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/rv_plic/rv_plic_gateway.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/rv_plic/plic_top.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/techmap/virtex7/mem/BRAM_1024x16.v
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/techmap/virtex7/mem/BRAM_16384x1.v
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/techmap/virtex7/mem/BRAM_2048x8.v
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/techmap/virtex7/mem/BRAM_4096x4.v
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/techmap/virtex7/mem/BRAM_512x32.v
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/techmap/virtex7/mem/BRAM_8192x2.v
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/techmap/virtex7/sgmii_vcu118_reset_sync.v
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_pmp.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_alu.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_branch_predict.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_compressed_decoder.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_controller.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_core_tracing.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_counter.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_cs_registers.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_csr.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_decoder.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_dummy_instr.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_ex_block.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_fetch_fifo.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_icache.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_id_stage.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_if_stage.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_load_store_unit.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_multdiv_fast.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_multdiv_slow.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_prefetch_buffer.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_register_file_ff.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_tracer_pkg.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_tracer.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_wb_stage.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/rtl/ibex_core.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/dv/uvm/core_ibex/common/prim/prim_pkg.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/dv/uvm/core_ibex/common/prim/prim_clock_gating.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/vendor/lowrisc_ip/ip/prim_generic/rtl/prim_generic_clock_gating.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/shared/rtl/timer.sv
	set_global_assignment -name SYSTEMVERILOG_FILE /home/yaagna/intel_esp/esp/rtl/cores/ibex/ibex/vendor/lowrisc_ip/ip/prim_xilinx/rtl/prim_xilinx_clock_gating.sv
	set_global_assignment -name SYSTEMVERILOG_FILE "/home/yaagna/intel_esp/esp/socs/xilinx-vc707-xc7vx485t/socgen/esp/plic_regmap.sv"
	set_global_assignment -name PROJECT_IP_REGENERATION_POLICY NEVER_REGENERATE_IP
	set_global_assignment -name VERILOG_INPUT_VERSION SYSTEMVERILOG_2012

	# Including default assignments
	set_global_assignment -name TIMING_ANALYZER_MULTICORNER_ANALYSIS ON -family "Cyclone 10 GX"
	set_global_assignment -name TDC_CCPP_TRADEOFF_TOLERANCE 0 -family "Cyclone 10 GX"
	set_global_assignment -name TIMING_ANALYZER_DO_CCPP_REMOVAL ON -family "Cyclone 10 GX"
	set_global_assignment -name SYNTH_TIMING_DRIVEN_SYNTHESIS ON -family "Cyclone 10 GX"
	set_global_assignment -name SYNCHRONIZATION_REGISTER_CHAIN_LENGTH 3 -family "Cyclone 10 GX"
	set_global_assignment -name SYNTH_RESOURCE_AWARE_INFERENCE_FOR_BLOCK_RAM ON -family "Cyclone 10 GX"
	set_global_assignment -name USE_ADVANCED_DETAILED_LAB_LEGALITY ON -family "Cyclone 10 GX"
	set_global_assignment -name ADVANCED_PHYSICAL_SYNTHESIS_REGISTER_PACKING OFF -family "Cyclone 10 GX"
	set_global_assignment -name PHYSICAL_SYNTHESIS OFF -family "Cyclone 10 GX"
	set_global_assignment -name STRATIXV_CONFIGURATION_SCHEME "PASSIVE SERIAL" -family "Cyclone 10 GX"
	set_global_assignment -name OPTIMIZE_HOLD_TIMING "ALL PATHS" -family "Cyclone 10 GX"
	set_global_assignment -name OPTIMIZE_MULTI_CORNER_TIMING ON -family "Cyclone 10 GX"
	set_global_assignment -name PROGRAMMABLE_POWER_TECHNOLOGY_SETTING AUTOMATIC -family "Cyclone 10 GX"
	set_global_assignment -name ENABLE_PHYSICAL_DSP_MERGING OFF -family "Cyclone 10 GX"
	set_global_assignment -name AUTO_DELAY_CHAINS ON -family "Cyclone 10 GX"
	set_global_assignment -name CRC_ERROR_OPEN_DRAIN ON -family "Cyclone 10 GX"
	set_global_assignment -name ACTIVE_SERIAL_CLOCK FREQ_100MHZ -family "Cyclone 10 GX"
	set_global_assignment -name GENERATE_PR_RBF_FILE OFF -family "Cyclone 10 GX"
	set_global_assignment -name POWER_USE_DEVICE_CHARACTERISTICS TYPICAL -family "Cyclone 10 GX"
	set_global_assignment -name POWER_AUTO_COMPUTE_TJ ON -family "Cyclone 10 GX"

	# Commit assignments
	export_assignments

	# Close project
	if {$need_to_close_project} {
		project_close
	}
}
