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

use work.register_config.all;

entity RegisterControl is
  generic (
    NBits     : integer := 4;
    NBitsEnc  : integer := 7
  );
  port (
    CLK             : in  std_logic;  -- 125 MHz (same as for ethernet transceiver)
    RESET           : in  std_logic;
    LED             : out std_logic_vector(7 downto 0);
    USER_SWITCH     : in  std_logic_vector(4 downto 0);

    -- register handling
    REG_EN          : in  std_logic;  -- register access
    REG_WR          : in  std_logic;  -- 0: read, 1: write
    REG_ADDR        : in  std_logic_vector(REG_ADDR_LEN-1 downto 0);  -- register address
    REG_DATA        : in  std_logic_vector(REG_LEN-1 downto 0);       -- data in
    REG_DATA_OUT    : out std_logic_vector(REG_LEN-1 downto 0);       -- data out
    REG_VALID       : out std_logic
  );
end RegisterControl;


architecture rtl of RegisterControl is

  -- registers
  signal r_led_config           : std_logic_vector(4 downto 0);


  signal one_hertz_counter      : std_logic_vector(7 downto 0);
  signal knight_rider           : std_logic_vector(7 downto 0);

begin

  -- ---------------------------------------------------------------------------
  --  LED output modules
  -- ---------------------------------------------------------------------------

  SECONDS_COUNTER : one_hertz_counter
  port map (
    CLK         => CLK,
    COUNTER_OUT => one_hertz_counter
  );

  KNIGHT_RIDER : knight_rider
  port map (
    CLK66       => CLK,
    USER_SWITCH => USER_SWITCH,
    LED         => knight_rider
  );


  -- ---------------------------------------------------------------------------
  --  internal register effects
  -- ---------------------------------------------------------------------------

  with r_led_config select LED <=
    one_hertz_counter         when std_logic_vector(to_unsigned(0, )),
    knight_rider              when std_logic_vector(to_unsigned(1, )),
    (others => '1')           when others;


  -- ---------------------------------------------------------------------------
  --  register writing
  -- ---------------------------------------------------------------------------

  single_register_write <= REG_EN and REG_WR;

  process (reset, clk)
  begin
    if reset = '1' then
      r_led_config <= (others => '0');

    elsif rising_edge(clk) and single_register_write = '1' then

      case REG_ADDR is
        when LED_REG =>
          r_led_config <= REG_DATA(r_led_config'range);

        when others =>
      end case;
    end if;
  end process;
end rtl;
