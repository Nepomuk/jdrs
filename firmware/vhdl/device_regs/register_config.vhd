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
  constant REG_LEN                : integer := 4*8;
  constant REG_BULK_LEN           : integer := 4*8;
  constant REG_ADDR_LEN           : integer := 12;

  -- ---------------------------------------------------------------------------
  --  Readout system registers
  -- ---------------------------------------------------------------------------

  -- general registers
  constant JDRS_IDENT             : integer := 16#000#;
  constant JDRS_STATUS            : integer := 16#001#;
  constant JDRS_CONTROL           : integer := 16#002#;

  -- some board features
  constant LED_REG                : integer := 16#010#;
  constant LCD_MODE_REG           : integer := 16#015#;
  constant LCD_FIFO_CHAR_REG      : integer := 16#016#;

  -- bulk data transfer
  constant BULK_DATA              : integer := 16#020#;
  constant BULK_DATA_COUNT        : integer := 16#021#;


  -- ---------------------------------------------------------------------------
  --  Device specific registers
  -- ---------------------------------------------------------------------------

end register_config;

package body register_config is
end register_config;
