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

    -- MMCM output
    CLK_MMCM        : out std_logic;
    RST_MMCM        : out std_logic;

    -- register handling
    REG_EN          : in  std_logic;  -- register access
    REG_WR          : in  std_logic;  -- 0: read, 1: write
    REG_ADDR        : in  std_logic_vector(REG_ADDR_LEN-1 downto 0);  -- register address
    REG_DATA        : in  std_logic_vector(REG_LEN-1 downto 0);       -- data in
    REG_DATA_OUT    : out std_logic_vector(REG_LEN-1 downto 0);       -- data out
    REG_VALID       : out std_logic;

    -- generic registers
    REG_DEV0_CONFIG : out reg_devX_config_type; -- generic config registers for device 0
    REG_DEV0_STATUS : in  reg_devX_status_type; -- generic status registers for device 0
    REG_DEV1_CONFIG : out reg_devX_config_type; -- generic config registers for device 1
    REG_DEV1_STATUS : in  reg_devX_status_type; -- generic status registers for device 1

    -- bulk transfer
    REG_BLK_EN      : in  std_logic;  -- bulk register access
    REG_BLK_RDEN    : in  std_logic;  -- read enable for dev0 daq fifo (comes from eth)
    REG_BLK_DOUT    : out std_logic_vector(REG_BULK_LEN-1 downto 0);        -- to eth
    REG_BLK_COUNT   : out std_logic_vector(BLK_FIFO_DEPTH_BITS-1 downto 0); -- fill level of fifo
    REG_BLK_EMPTY   : out std_logic;  -- the selected daq fifo is empty
    REG_BLK_VALID   : out std_logic;  -- do we have valid data on the output line?

    BLK_DEV0_WREN   : in  std_logic;  -- write enable for dev0 daq fifo (comes from dev)
    BLK_DEV0_DIN    : in  std_logic_vector(REG_BULK_LEN-1 downto 0)         -- from dev
  );
end RegisterControl;


architecture rtl of RegisterControl is

  component daq_fifo
    port (
      RST           : in  std_logic;
      WR_CLK        : in  std_logic;
      RD_CLK        : in  std_logic;
      DIN           : in  std_logic_vector(REG_BULK_LEN-1 downto 0);
      WR_EN         : in  std_logic;
      RD_EN         : in  std_logic;
      DOUT          : out std_logic_vector(REG_BULK_LEN-1 downto 0);
      FULL          : out std_logic;
      EMPTY         : out std_logic;
      RD_DATA_COUNT : out std_logic_vector(BLK_FIFO_DEPTH_BITS-1 downto 0)
    );
  end component;

  component fifo_fill_counter
    port (
      CLK : in  std_logic;
      CE  : in  std_logic;
      Q   : out std_logic_vector(31 downto 0)
    );
  end component;


  -- internal control signals
  signal single_register_read   : std_logic;
  signal single_register_write  : std_logic;
  signal register_address       : integer range 0 to 2**REG_ADDR_LEN-1;
  signal register_address_scope : integer range 0 to 2**(REG_ADDR_LEN-8)-1;
  signal register_address_dev   : integer range 0 to 2**8-1;
  signal register_read          : std_logic_vector(REG_LEN-1 downto 0);

  -- MMCM signals
  signal mmcm_clk               : std_logic;
  signal mmcm_reset             : std_logic;
  signal mmcm_start             : std_logic;
  signal mmcm_wren              : std_logic;
  signal mmcm_data_out          : std_logic_vector(15 downto 0);

  -- DAQ fifo
  signal dev0_fifo_wren         : std_logic;
  signal dev0_fifo_rden         : std_logic;
  signal dev0_fifo_empty        : std_logic;
  signal dev0_fifo_full         : std_logic;
  signal dev0_fifo_din          : std_logic_vector(REG_BULK_LEN-1 downto 0);
  signal dev0_fifo_dout         : std_logic_vector(REG_BULK_LEN-1 downto 0);
  signal dev0_fifo_count        : std_logic_vector(BLK_FIFO_DEPTH_BITS-1 downto 0);
  signal dev0_fifo_single_read  : std_logic;
  signal dev0_fifo_single_read_d1 : std_logic;

  -- registers
  signal r_led_config           : std_logic_vector(4 downto 0);
  signal r_dev0_config          : reg_devX_config_type;
  signal r_dev1_config          : reg_devX_config_type;
  signal r_dev0_fifo_fillType   : std_logic_vector(2 downto 0) := "001";


  signal bulk_device_select     : integer range 0 to 1 := 0;
  signal dev0_fifo_blk_en       : std_logic;
  signal dev0_fifo_fill_ctr     : std_logic_vector(REG_BULK_LEN-1 downto 0);
  signal dev0_fifo_fill_ctr_en  : std_logic;

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
    CLK66       => mmcm_clk, --CLK,
    USER_SWITCH => USER_SWITCH,
    LED         => knight_rider
  );


  -- ---------------------------------------------------------------------------
  --  MMCM clock generator
  -- ---------------------------------------------------------------------------

  MMCM : entity work.U_MMCM
  port map (
    CLKIN     => CLK,
    LCLK      => open,
    LRESET    => open,
    RC_A      => REG_ADDR(4 downto 0),
    RC_DO     => mmcm_data_out,
    RC_DI     => REG_DATA(15 downto 0),
    RC_WE     => mmcm_wren,
    RC_START  => mmcm_start,
    RC_CLK    => mmcm_clk,
    RC_CLK180 => open,
    RC_RESET  => mmcm_reset
  );

  mmcm_wren <= '1' when
      single_register_write = '1' and
      register_address >= RA_MMCM_START and
      register_address < RA_MMCM_START + N_MMCM_REGISTERS
    else '0';

  mmcm_start <= '1' when
      single_register_write = '1' and
      register_address = RA_MMCM_START
    else '0';

  CLK_MMCM <= mmcm_clk;
  RST_MMCM <= mmcm_reset;


  -- ---------------------------------------------------------------------------
  --  DAQ data fifo for bulk transfer
  -- ---------------------------------------------------------------------------

  dev0_daq_fifo : daq_fifo
  port map (
    RST           => RESET,
    WR_CLK        => mmcm_clk,
    RD_CLK        => CLK,
    DIN           => dev0_fifo_din,
    WR_EN         => dev0_fifo_wren,
    RD_EN         => dev0_fifo_rden,
    DOUT          => dev0_fifo_dout,
    FULL          => dev0_fifo_full,
    EMPTY         => dev0_fifo_empty,
    RD_DATA_COUNT => dev0_fifo_count
  );


  -- fill the fifo
  dev0_fifo_wren <= not dev0_fifo_full when r_dev0_fifo_fillType = "001" else
      BLK_DEV0_WREN and not dev0_fifo_full;

  dev0_fifo_din <= dev0_fifo_fill_ctr when r_dev0_fifo_fillType = "001" else
      BLK_DEV0_DIN;


  dev0_fifo_fill_ctr_en <= '1' when r_dev0_fifo_fillType = "001" and dev0_fifo_full = '0' else '0';
  dev0_fifo_fill_counter : fifo_fill_counter
  port map (
    CLK => mmcm_clk,
    CE  => dev0_fifo_fill_ctr_en,
    Q   => dev0_fifo_fill_ctr
  );


  -- read from the fifo
  dev0_fifo_single_read <= '1' when single_register_read = '1' and
      register_address = RA_DEV0_BULK_DATA and dev0_fifo_empty = '0' else '0';

  process ( CLK ) begin
    if rising_edge( CLK ) then
      dev0_fifo_single_read_d1 <= dev0_fifo_single_read;
    end if;
  end process;

  dev0_fifo_blk_en <= '1' when REG_BLK_EN = '1' and bulk_device_select = 0 else '0';

  dev0_fifo_rden <= (
      (dev0_fifo_blk_en and REG_BLK_RDEN) or                    -- regular bulk transfer
      (dev0_fifo_single_read and not dev0_fifo_single_read_d1)  -- read only one word
    ) and not dev0_fifo_empty;


  -- bulk output multiplexer
  REG_BLK_DOUT <= dev0_fifo_dout when dev0_fifo_blk_en = '1' else
    (others => '0');

  REG_BLK_COUNT <= dev0_fifo_count when dev0_fifo_blk_en = '1' else
    (others => '0');

  REG_BLK_EMPTY <= dev0_fifo_empty when dev0_fifo_blk_en = '1' else
    '0';

  -- TODO: meaningful valid signal
  REG_BLK_VALID <= dev0_fifo_blk_en;

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
      dev0_fifo_single_read = '1' or
      register_address_scope = DEV0_SCOPE or
      register_address_scope = DEV1_SCOPE
    ) else '0';


  REG_DATA_OUT <=
    EXT2SLV(r_led_config) when single_register_read = '1' and register_address = RA_LED_REG else
    EXT2SLV(dev0_fifo_dout) when dev0_fifo_single_read = '1' else

    -- MMCM data out
    EXT2SLV(mmcm_data_out) when single_register_read = '1' and (
        register_address >= RA_MMCM_START and
        register_address < RA_MMCM_START + N_MMCM_REGISTERS
      ) else

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
