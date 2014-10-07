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
    REG_VALID       : out std_logic;

    REG_DEV0_CONFIG : out reg_devX_config_type; -- generic config registers for device 0
    REG_DEV0_STATUS : in  reg_devX_status_type; -- generic status registers for device 0
    REG_DEV1_CONFIG : out reg_devX_config_type; -- generic config registers for device 1
    REG_DEV1_STATUS : in  reg_devX_status_type  -- generic status registers for device 1
  );
end RegisterControl;


architecture rtl of RegisterControl is

  -- internal control signals
  signal single_register_read   : std_logic;
  signal single_register_write  : std_logic;
  signal register_address       : integer range 0 to 2**REG_ADDR_LEN-1;
  signal register_address_scope : integer range 0 to 2**(REG_ADDR_LEN-8)-1;
  signal register_address_dev   : integer range 0 to 2**8-1;
  signal register_read          : std_logic_vector(REG_LEN-1 downto 0);

  -- registers
  signal r_led_config           : std_logic_vector(4 downto 0);
  signal r_dev0_config          : reg_devX_config_type;
  signal r_dev1_config          : reg_devX_config_type;


  signal one_hertz_counter      : std_logic_vector(7 downto 0);
  signal knight_rider           : std_logic_vector(7 downto 0);

begin

  -- ---------------------------------------------------------------------------
  --  LED output modules
  -- ---------------------------------------------------------------------------

  SECONDS_COUNTER : entity work.one_hertz_counter
  port map (
    CLK         => CLK,
    COUNTER_OUT => one_hertz_counter
  );

  LED_KNIGHT_RIDER : entity work.knight_rider
  port map (
    CLK66       => CLK,
    USER_SWITCH => USER_SWITCH,
    LED         => knight_rider
  );


  -- ---------------------------------------------------------------------------
  --  internal register effects
  -- ---------------------------------------------------------------------------

  with r_led_config select LED <=
    one_hertz_counter         when std_logic_vector(to_unsigned(0, r_led_config'length)),
    knight_rider              when std_logic_vector(to_unsigned(1, r_led_config'length)),
    (others => '1')           when others;


  -- ---------------------------------------------------------------------------
  --  register control signals
  -- ---------------------------------------------------------------------------

  single_register_read  <= REG_EN and not REG_WR;
  single_register_write <= REG_EN and REG_WR;
  register_address        <= to_integer(unsigned(REG_ADDR));
  register_address_scope  <= to_integer(unsigned(REG_ADDR(REG_ADDR_LEN-1 downto 8)));
  register_address_dev    <= to_integer(unsigned(REG_ADDR(7 downto 0)));


  -- ---------------------------------------------------------------------------
  --  register writing
  -- ---------------------------------------------------------------------------

  process (reset, clk)
  begin
    if reset = '1' then
      r_led_config <= (others => '0');

    elsif rising_edge(clk) and single_register_write = '1' then

      -- first device registers
      if register_address_scope = DEV0_SCOPE and
          register_address_dev >= DEVX_CONFIG_START and
          register_address_dev <= DEVX_CONFIG_END then
        r_dev0_config(register_address_dev) <= REG_DATA;

      -- second device registers
      elsif register_address_scope = DEV1_SCOPE and
          register_address_dev >= DEVX_CONFIG_START and
          register_address_dev <= DEVX_CONFIG_END then
        r_dev1_config(register_address_dev) <= REG_DATA;

      -- everything else
      else
        case register_address is
          when RA_LED_REG =>
            r_led_config <= REG_DATA(r_led_config'range);

          when others =>
        end case;
      end if;
    end if;
  end process;

  REG_DEV0_CONFIG <= r_dev0_config;
  REG_DEV1_CONFIG <= r_dev1_config;


  -- ---------------------------------------------------------------------------
  --  register reading
  -- ---------------------------------------------------------------------------

  REG_VALID <= '1' when single_register_read = '1' and (
      register_address = RA_LED_REG or
      register_address_scope = DEV0_SCOPE or
      register_address_scope = DEV1_SCOPE
    ) else '0';

  REG_DATA_OUT <=
    EXT2SLV(r_led_config) when single_register_read = '1' and register_address = RA_LED_REG else

    -- generic device registers
    r_dev0_config(register_address_dev) when single_register_read = '1' and (
        register_address_scope = DEV0_SCOPE and
        register_address_dev >= DEVX_CONFIG_START and
        register_address_dev <= DEVX_CONFIG_END ) else

    REG_DEV0_STATUS(register_address_dev) when single_register_read = '1' and (
        register_address_scope = DEV0_SCOPE and
        register_address_dev >= DEVX_STATUS_START and
        register_address_dev <= DEVX_STATUS_END ) else

    r_dev1_config(register_address_dev) when single_register_read = '1' and (
        register_address_scope = DEV1_SCOPE and
        register_address_dev >= DEVX_CONFIG_START and
        register_address_dev <= DEVX_CONFIG_END ) else

    REG_DEV1_STATUS(register_address_dev) when single_register_read = '1' and (
        register_address_scope = DEV1_SCOPE and
        register_address_dev >= DEVX_STATUS_START and
        register_address_dev <= DEVX_STATUS_END ) else

    (others => '0');


end rtl;
