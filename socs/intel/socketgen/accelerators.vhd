-- Copyright (c) 2011-2024 Columbia University, System Level Design Group
-- SPDX-License-Identifier: Apache-2.0


library ieee;
use ieee.std_logic_1164.all;
use work.sld_devices.all;
use work.allacc.all;

entity fuseml_rtl_rtl is

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

end entity fuseml_rtl_rtl;


architecture mapping of fuseml_rtl_rtl is

begin  -- mapping


  impl_basic_dma64_gen: if hls_conf = HLSCFG_FUSEML_RTL_BASIC_DMA64 generate
    fuseml_rtl_basic_dma64_i: fuseml_rtl_basic_dma64
    port map(
      conf_info_reg10            => conf_info_reg10,
      conf_info_reg8             => conf_info_reg8,
      conf_info_reg9             => conf_info_reg9,
      conf_info_reg2             => conf_info_reg2,
      conf_info_reg3             => conf_info_reg3,
      conf_info_reg1             => conf_info_reg1,
      conf_info_reg6             => conf_info_reg6,
      conf_info_reg7             => conf_info_reg7,
      conf_info_reg4             => conf_info_reg4,
      conf_info_reg5             => conf_info_reg5,
      clk                        => clk,
      rst                        => acc_rst,
      conf_done                  => conf_done,
      dma_read_ctrl_valid        => dma_read_ctrl_valid,
      dma_read_ctrl_ready        => dma_read_ctrl_ready,
      dma_read_ctrl_data_index   => dma_read_ctrl_data_index,
      dma_read_ctrl_data_length  => dma_read_ctrl_data_length,
      dma_read_ctrl_data_size    => dma_read_ctrl_data_size,
      dma_write_ctrl_valid       => dma_write_ctrl_valid,
      dma_write_ctrl_ready       => dma_write_ctrl_ready,
      dma_write_ctrl_data_index  => dma_write_ctrl_data_index,
      dma_write_ctrl_data_length => dma_write_ctrl_data_length,
      dma_write_ctrl_data_size   => dma_write_ctrl_data_size,
      dma_read_chnl_valid        => dma_read_chnl_valid,
      dma_read_chnl_ready        => dma_read_chnl_ready,
      dma_read_chnl_data         => dma_read_chnl_data,
      dma_write_chnl_valid       => dma_write_chnl_valid,
      dma_write_chnl_ready       => dma_write_chnl_ready,
      dma_write_chnl_data        => dma_write_chnl_data,
      acc_done                   => acc_done
    );
  end generate impl_basic_dma64_gen;


  impl_impl_gen: if hls_conf = HLSCFG_FUSEML_RTL_IMPL generate
    fuseml_rtl_impl_i: fuseml_rtl_impl
    port map(
      conf_info_reg10            => conf_info_reg10,
      conf_info_reg8             => conf_info_reg8,
      conf_info_reg9             => conf_info_reg9,
      conf_info_reg2             => conf_info_reg2,
      conf_info_reg3             => conf_info_reg3,
      conf_info_reg1             => conf_info_reg1,
      conf_info_reg6             => conf_info_reg6,
      conf_info_reg7             => conf_info_reg7,
      conf_info_reg4             => conf_info_reg4,
      conf_info_reg5             => conf_info_reg5,
      clk                        => clk,
      rst                        => acc_rst,
      conf_done                  => conf_done,
      dma_read_ctrl_valid        => dma_read_ctrl_valid,
      dma_read_ctrl_ready        => dma_read_ctrl_ready,
      dma_read_ctrl_data_index   => dma_read_ctrl_data_index,
      dma_read_ctrl_data_length  => dma_read_ctrl_data_length,
      dma_read_ctrl_data_size    => dma_read_ctrl_data_size,
      dma_write_ctrl_valid       => dma_write_ctrl_valid,
      dma_write_ctrl_ready       => dma_write_ctrl_ready,
      dma_write_ctrl_data_index  => dma_write_ctrl_data_index,
      dma_write_ctrl_data_length => dma_write_ctrl_data_length,
      dma_write_ctrl_data_size   => dma_write_ctrl_data_size,
      dma_read_chnl_valid        => dma_read_chnl_valid,
      dma_read_chnl_ready        => dma_read_chnl_ready,
      dma_read_chnl_data         => dma_read_chnl_data,
      dma_write_chnl_valid       => dma_write_chnl_valid,
      dma_write_chnl_ready       => dma_write_chnl_ready,
      dma_write_chnl_data        => dma_write_chnl_data,
      acc_done                   => acc_done
    );
  end generate impl_impl_gen;

end mapping;

