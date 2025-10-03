-- Copyright (c) 2011-2024 Columbia University, System Level Design Group
-- SPDX-License-Identifier: Apache-2.0

library ieee;
use ieee.std_logic_1164.all;
use work.sld_devices.all;
use work.esp_global.all;

package genacc is


  component fuseml_rtl_rtl
    generic (
      hls_conf  : hlscfg_t
    );

    port (
      conf_info_reg10            : in  std_logic_vector(31 downto 0);
      conf_info_reg8             : in  std_logic_vector(31 downto 0);
      conf_info_reg9             : in  std_logic_vector(31 downto 0);
      conf_info_reg2             : in  std_logic_vector(31 downto 0);
      conf_info_reg3             : in  std_logic_vector(31 downto 0);
      conf_info_reg1             : in  std_logic_vector(31 downto 0);
      conf_info_reg6             : in  std_logic_vector(31 downto 0);
      conf_info_reg7             : in  std_logic_vector(31 downto 0);
      conf_info_reg4             : in  std_logic_vector(31 downto 0);
      conf_info_reg5             : in  std_logic_vector(31 downto 0);
      clk                        : in  std_ulogic;
      acc_rst                    : in  std_ulogic;
      conf_done                  : in  std_ulogic;
      dma_read_ctrl_valid        : out std_ulogic;
      dma_read_ctrl_ready        : in  std_ulogic;
      dma_read_ctrl_data_index   : out std_logic_vector(31 downto 0);
      dma_read_ctrl_data_length  : out std_logic_vector(31 downto 0);
      dma_read_ctrl_data_size    : out std_logic_vector(2 downto 0);
      dma_write_ctrl_valid       : out std_ulogic;
      dma_write_ctrl_ready       : in  std_ulogic;
      dma_write_ctrl_data_index  : out std_logic_vector(31 downto 0);
      dma_write_ctrl_data_length : out std_logic_vector(31 downto 0);
      dma_write_ctrl_data_size   : out std_logic_vector(2 downto 0);
      dma_read_chnl_valid        : in  std_ulogic;
      dma_read_chnl_ready        : out std_ulogic;
      dma_read_chnl_data         : in  std_logic_vector(63 downto 0);
      dma_write_chnl_valid       : out std_ulogic;
      dma_write_chnl_ready       : in  std_ulogic;
      dma_write_chnl_data        : out std_logic_vector(63 downto 0);
      acc_done                   : out std_ulogic
    );
  end component;



  component NV_NVDLA_wrapper
    port (
  dla_core_clk : in std_logic;
  dla_csb_clk : in std_logic;
  dla_reset_rstn : in std_logic;
  direct_reset : in std_logic;
  psel : in std_ulogic;
  penable : in std_ulogic;
  paddr : in std_logic_vector(31 downto 0);
  pwrite : in std_ulogic;
  pwdata : in std_logic_vector(31 downto 0);
  prdata : out std_Logic_vector(31 downto 0);
  pready : out std_logic;
  pslverr : out std_logic;
  nvdla_core2dbb_awid : out std_logic_vector(8 - 1 downto 0);
  nvdla_core2dbb_awaddr : out std_logic_vector(32 - 1 downto 0);
  nvdla_core2dbb_awlen : out std_logic_vector(7 downto 0);
  nvdla_core2dbb_awsize : out std_logic_vector(2 downto 0);
  nvdla_core2dbb_awburst : out std_logic_vector(1 downto 0);
  nvdla_core2dbb_awlock : out std_logic;
  nvdla_core2dbb_awcache : out std_logic_vector(3 downto 0);
  nvdla_core2dbb_awprot : out std_logic_vector(2 downto 0);
  nvdla_core2dbb_awvalid : out std_logic;
  nvdla_core2dbb_awqos : out std_logic_vector(3 downto 0);
  nvdla_core2dbb_awatop : out std_logic_vector(5 downto 0);
  nvdla_core2dbb_awregion : out std_logic_vector(3 downto 0);
  nvdla_core2dbb_awready : in std_logic;
  nvdla_core2dbb_wdata : out std_logic_vector (64 - 1 downto 0);
  nvdla_core2dbb_wstrb : out std_logic_vector (64/8 - 1 downto 0);
  nvdla_core2dbb_wlast : out std_logic;
  nvdla_core2dbb_wvalid : out std_logic;
  nvdla_core2dbb_wready : in std_logic;
  nvdla_core2dbb_arid  : out std_logic_vector (8 - 1 downto 0);
  nvdla_core2dbb_araddr : out std_logic_vector (32 - 1 downto 0);
  nvdla_core2dbb_arlen : out std_logic_vector (7 downto 0);
  nvdla_core2dbb_arsize : out std_logic_vector (2 downto 0);
  nvdla_core2dbb_arburst : out std_logic_vector (1 downto 0);
  nvdla_core2dbb_arlock : out std_logic;
  nvdla_core2dbb_arcache : out std_logic_vector (3 downto 0);
  nvdla_core2dbb_arprot : out std_logic_vector (2 downto 0);
  nvdla_core2dbb_arvalid : out std_logic;
  nvdla_core2dbb_arqos : out std_logic_vector (3 downto 0);
  nvdla_core2dbb_arregion : out std_logic_vector(3 downto 0);
  nvdla_core2dbb_arready : in std_logic;
  nvdla_core2dbb_rready : out std_logic;
  nvdla_core2dbb_rid : in std_logic_vector (8 - 1 downto 0);
  nvdla_core2dbb_rdata : in std_logic_vector (64 - 1 downto 0);
  nvdla_core2dbb_rresp : in std_logic_vector (1 downto 0);
  nvdla_core2dbb_rlast : in std_logic;
  nvdla_core2dbb_rvalid : in std_logic;
  nvdla_core2dbb_bready : out std_logic;
  nvdla_core2dbb_bid : in std_logic_vector (8-1 downto 0);
  nvdla_core2dbb_bresp : in std_logic_vector (1 downto 0);
  nvdla_core2dbb_bvalid : in std_logic;
  dla_intr : out std_logic
    );
  end component;



end;

