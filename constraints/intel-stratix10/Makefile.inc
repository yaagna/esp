# Copyright (c) 2011-2024 Columbia University, System Level Design Group
# SPDX-License-Identifier: Apache-2.0
# FPGA and Dev Board used in Design
PART=10CX220YF        # this is cyclone 10 gx device 
PACKAGE=780I5G        # this is Cyclone 10 gx device
# PROTOBOARD=xilinx.com:vc707:part0:1.4
SPEED=2
PROMGENPAR=-u 0 $(TOP).bit -p mcs -data_width 16 -w -o $(BOARD)
