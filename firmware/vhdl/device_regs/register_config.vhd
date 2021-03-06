--------------------------------------------------------------------------------
-- Author:   André Goerres (FZ Jülich) - a.goerres@fz-juelich.de
--
-- Create Date:  2014-09-30
-- Design Name:  JDRS
-- Description:
--
--------------------------------------------------------------------------------

library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

package register_config is

  -- ---------------------------------------------------------------------------
  --  Global configuration
  -- ---------------------------------------------------------------------------

  -- use mulitple of 8 bits for length
  -- Note: a bulk length which less than 4 bytes causes problems.
  constant REG_LEN_BYTES          : integer := 4;
  constant REG_LEN                : integer := REG_LEN_BYTES*8;
  constant REG_BULK_LEN_BYTES     : integer := 4;  -- if you change this, remember to recompile the fifo and fill counter
  constant REG_BULK_LEN           : integer := REG_BULK_LEN_BYTES*8;
  constant REG_ADDR_LEN           : integer := 12;
  constant BLK_FIFO_DEPTH_BITS    : integer := 16; -- number of bits needed to address the depth of the fifo

  -- ---------------------------------------------------------------------------
  --  Readout system registers
  -- ---------------------------------------------------------------------------

  -- all register addresses should have RA_ as a prefix to avoid multiple declarations

  -- general registers
  constant RA_JDRS_IDENT          : integer := 16#000#;
  constant RA_JDRS_STATUS         : integer := 16#001#;
  constant RA_JDRS_CONTROL        : integer := 16#002#;

  -- some board features
  constant RA_LED_REG             : integer := 16#010#;
  constant RA_LED_KNIGHT_RIDER    : integer := 16#011#;
  constant RA_LCD_MODE_REG        : integer := 16#015#;
  constant RA_LCD_FIFO_CHAR_REG   : integer := 16#016#;
  constant RA_SPILL_ACTIVE        : integer := 16#01a#; -- register holds the information if beam is available

  -- MMCM clock generator (32 addresses)
  constant RA_MMCM_START          : integer := 16#020#; -- up to 16#03F#
  constant N_MMCM_REGISTERS       : integer := 32;      -- number of MMCM registers

  -- bulk data transfer
  constant RA_DEV0_BULK_DATA        : integer := 16#040#; -- read data from device 0 fifo
  constant RA_DEV0_BULK_DATA_COUNT  : integer := 16#041#; -- how many data are in the fifo?
  constant RA_DEV0_BULK_DATA_FILL   : integer := 16#042#; -- with what should we fill the fifo?


  -- ---------------------------------------------------------------------------
  --  Generic device registers
  -- ---------------------------------------------------------------------------

  -- Config registers are set by the user and are output registers of
  -- register_config.vhd while status registers are set by the device and thus
  -- are inputs to register_config.vhd.

  constant DEV0_SCOPE             : integer := 16#1#; -- all adresses from 16x100 until 16x1FF
  constant DEV1_SCOPE             : integer := 16#2#; -- all adresses from 16x200 until 16x2FF

  constant DEVX_CONFIG_START      : integer := 16#00#;  -- this should be 0, otherwise work on register_control.vhd is needed
  constant DEVX_CONFIG_END        : integer := 16#7F#;
  constant DEVX_CONFIG_LEN        : integer := DEVX_CONFIG_END - DEVX_CONFIG_START;

  constant DEVX_STATUS_START      : integer := 16#80#;
  constant DEVX_STATUS_END        : integer := 16#FF#;  -- this should be FF, otherwise work on register_control.vhd is needed
  constant DEVX_STATUS_LEN        : integer := DEVX_STATUS_END - DEVX_STATUS_START;

  type reg_devX_config_type is array (DEVX_CONFIG_LEN-1 downto 0) of std_logic_vector(REG_LEN-1 downto 0);
  subtype reg_devX_cfg_wr_type is std_logic_vector(DEVX_CONFIG_LEN-1 downto 0);
  type reg_devX_status_type is array (DEVX_STATUS_LEN-1 downto 0) of std_logic_vector(REG_LEN-1 downto 0);
  subtype reg_devX_st_rd_type  is std_logic_vector(DEVX_STATUS_LEN-1 downto 0);


  -- ---------------------------------------------------------------------------
  --  Utility functions
  -- ---------------------------------------------------------------------------

  function EXT2SLV (
    SLV : std_logic_vector;
    n   : integer := REG_LEN
  ) return std_logic_vector;

  function Z2SLV (
    size  : integer
  ) return std_logic_vector;

end register_config;


-- -----------------------------------------------------------------------------
--  Package body
-- -----------------------------------------------------------------------------

package body register_config is

  -- extend a std_logic_vector to a given length
  function EXT2SLV (
    SLV : std_logic_vector;
    n   : integer := REG_LEN
  ) return std_logic_vector is
    variable result: std_logic_vector(n-1 downto 0);
  begin
    if ( SLV'length = n ) then
      result := SLV;
    elsif ( SLV'length < n ) then
      result := Z2SLV(n - SLV'length) & SLV;
    else
      result := SLV(result'range);
    end if;

    return result;
  end;


  -- return a std_logic_vector with zeros
  function Z2SLV (
    size  : integer
  ) return std_logic_vector is
    variable result: std_logic_vector(size-1 downto 0);
  begin
    result := (others => '0');
    return result;
  end;

end register_config;
