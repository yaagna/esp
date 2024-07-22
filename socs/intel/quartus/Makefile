##################  project specific targets ##########################

qwiz:
	@if [ ! -d ./ddr3ctrl/ ]; then \
		qmegawiz -silent ddr3ctrl.vhd ; \
	else \
		echo "DDR3 controller core already compiled"; \
	fi ;

localclean:
	@-rm -rf ddr3ctrl ddr3ctrl_example_design ddr3ctrl_sim
	@-rm -f ddr3ctrl.bsf ddr3ctrl.cmp ddr3ctrl_p0_all_pins.txt ddr3ctrl.ppf
	@-rm -f ddr3ctrl.qip ddr3ctrl_sim.f ddr3ctrl.sip ddr3ctrl.sopcinfo
	@-rm -f ddr3ctrl.spd
