-- Copyright (c) 2011-2024 Columbia University, System Level Design Group
-- SPDX-License-Identifier: Apache-2.0

library ieee;
use ieee.std_logic_1164.all;

package allacc is


  component fuseml_rtl_basic_dma64
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
      rst                        : in  std_ulogic;
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



  component fuseml_rtl_impl
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
      rst                        : in  std_ulogic;
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



end;
