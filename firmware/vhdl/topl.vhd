----------------------------------------------------------------------------------
-- Company:
-- Engineer:
--
-- Create Date:    11:38:35 05/31/2012
-- Design Name:
-- Module Name:    topl - Behavioral
-- Project Name:
-- Target Devices:
-- Tool versions:
-- Description:
--
-- Dependencies:
--
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
----------------------------------------------------------------------------------

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;

-- needed for register handling
use work.register_config.all;


entity topl is
  port (
    -- asynchronous reset
    GLBL_RST            : in  std_logic;

    -- 200MHz clock input from board
    CLK_IN_P            : in  std_logic;  		--! Differential on-board 200 MHz clock
    CLK_IN_N            : in  std_logic;			--! Differential on-board 200 MHz clock

    -- 125MHz GTX clock
    -- MGTREFCLK_N         : in  std_logic;
    -- MGTREFCLK_P         : in  std_logic;

    PHY_RESETN          : out std_logic;
    SM_FAN_PWM          : out std_logic;

    -- LCD  interface
    ------------------
    SF_D                : out std_logic_vector(3 downto 0); --! LCD data bus
    LCD_E               : out std_logic;              --! LCD: E   (control bit)
    LCD_RS              : out std_logic;              --! LCD: RS  (setup or data)
    LCD_RW              : out std_logic;              --! LCD: R/W (read or write)

    -- GMII Interface
    -----------------

    GMII_TXD            : out std_logic_vector(7 downto 0);
    GMII_TX_EN          : out std_logic;
    GMII_TX_ER          : out std_logic;
    GMII_TX_CLK         : out std_logic;
    GMII_RXD            : in  std_logic_vector(7 downto 0);
    GMII_RX_DV          : in  std_logic;
    GMII_RX_ER          : in  std_logic;
    GMII_RX_CLK         : in  std_logic;
    GMII_COL            : in  std_logic;
    GMII_CRS            : in  std_logic;
    MII_TX_CLK          : in  std_logic;

    -- Design controls and output
    -------------------------------
    PUSHBTN_SW          : in  std_logic_vector (4 downto 0);   --! 5 Push buttons
    PUSHBTN_LED         : out std_logic_vector (4 downto 0);   --! 5 Push buttons LEDs
    USER_LED            : out std_logic_vector (7 downto 0);   --! 8 GPIO LEDs
    USER_SWITCH         : in  std_logic_vector (7 downto 0);   --! 8 GPIO Switches

    SMA_GPIO_N          : in  std_logic
  );
end topl;




architecture Behavioral of topl is

  signal refclk_bufg    : std_logic;
  signal gtx_clk_bufg   : std_logic;
  signal clk_locked     : std_logic;
  signal clk_50         : std_logic;

  -- the register information bus
  signal register_access        : std_logic;
  signal register_access_ready  : std_logic;
  signal register_write_or_read : std_logic; -- 0: read, 1: write
  signal register_addr          : std_logic_vector(15 downto 0);
  signal register_read_data     : std_logic_vector(31 downto 0);
  signal register_write_data    : std_logic_vector(31 downto 0);

  signal register_blk_en        : std_logic;
  signal register_blk_rden      : std_logic;
  signal register_blk_data      : std_logic_vector(REG_BULK_LEN-1 downto 0);
  signal register_blk_count     : std_logic_vector(BLK_FIFO_DEPTH_BITS-1 downto 0);
  signal register_blk_empty     : std_logic;
  signal register_blk_valid     : std_logic;

  -- generic device registers
  signal reg_dev0_config        : reg_devX_config_type;
  signal reg_dev0_cfg_wr        : reg_devX_cfg_wr_type;
  signal reg_dev0_status        : reg_devX_status_type;
  signal reg_dev0_st_rd         : reg_devX_st_rd_type;
  signal reg_dev1_config        : reg_devX_config_type;
  signal reg_dev1_cfg_wr        : reg_devX_cfg_wr_type;
  signal reg_dev1_status        : reg_devX_status_type;
  signal reg_dev1_st_rd         : reg_devX_st_rd_type;

  -- LCD stuff
  constant lcd_mode_default : std_logic_vector(2 downto 0) := "001";
  signal lcd_ctrl           : std_logic_vector(2 downto 0);
  signal lcd_mode           : std_logic_vector(2 downto 0) := lcd_mode_default;
  signal lcd_db             : std_logic_vector(7 downto 4);

  -- ouput for the LCD
  signal temp_int           : std_logic_vector(7 downto 0);
  signal temp_adc_int       : std_logic_vector(9 downto 0);
  signal fan_speed_int      : std_logic_vector(5 downto 0);
  signal udp_pkg_ctr        : std_logic_vector(31 downto 0);

  -- make the register information visible on the LCD for some clock cycles
  constant lcd_enable_register_display    : std_logic := '1';
  constant register_display_counter_max   : integer := 125000000*2;
  signal register_display_counter         : integer range 0 to register_display_counter_max := 0;
  signal register_display_counter_enable  : std_logic := '0';
  signal register_display_counter_running : std_logic := '0';

  constant lcd_enable_eth_display         : std_logic := '1';
  constant eth_display_counter_max        : integer := 125000000*10;
  signal eth_display_counter              : integer range 0 to eth_display_counter_max := 0;
  signal eth_display_counter_enable       : std_logic := '0';
  signal eth_display_counter_running      : std_logic := '0';
  signal eth_switch                       : std_logic_vector(3 downto 0);
  signal eth_switch_buffer                : std_logic_vector(3 downto 0);
  signal eth_switch_change                : std_logic;
  signal eth_ip                           : std_logic_vector(31 downto 0);
  signal eth_mac                          : std_logic_vector(47 downto 0);

  -- register buffer signals, since the information
  -- should be visible on the LCD more than a clock cycle
  signal register_addr_buf          : std_logic_vector(11 downto 0);
  signal register_write_or_read_buf : std_logic; -- 0: read, 1: write
  signal register_data_buf          : std_logic_vector(31 downto 0);


begin

  -- Unused LEDs
  PUSHBTN_LED(4 downto 1) <= (others => '0');

  ------------------------------------------------------------------------------
  -- Clock generator
  ------------------------------------------------------------------------------

  CLOCK_GENERATOR : entity work.clk_wiz_v3_3
  port map (
    -- Clock in ports
    CLK_IN_P      => CLK_IN_P,
    CLK_IN_N      => CLK_IN_N,

    -- Clock out ports
    CLK_OUT_200   => refclk_bufg,
    CLK_OUT_125   => gtx_clk_bufg,
    CLK_OUT_50    => clk_50,

    -- Status and control signals
    RESET         => GLBL_RST,
    LOCKED        => clk_locked
  );


  ------------------------------------------------------------------------------
  -- Fan regulator
  ------------------------------------------------------------------------------

  FAN_REGULATOR : entity work.fan_regulator
  port map (
    CLK           => clk_50,
    RESET         => GLBL_RST,
    FAN_PWM       => SM_FAN_PWM,

    TEMP_OUT      => temp_int,
    TEMP_ADC_OUT  => temp_adc_int,
    FAN_SPEED_OUT => fan_speed_int
  );


  ------------------------------------------------------------------------------
  -- LCD control module
  ------------------------------------------------------------------------------

  LCD_CONTROL : entity work.lcd_control
  port map (
    RST           => GLBL_RST,
    CLK           => clk_50,
    MODE          => lcd_mode,
    CONTROL       => lcd_ctrl,  -- LCD_RS, LCD_RW, LCD_E
    SF_D          => lcd_db,    -- LCD data bus

    TEMP_IN       => temp_int,
    TEMP_ADC_IN   => temp_adc_int,
    FAN_SPEED_IN  => fan_speed_int,

    REGISTER_ADDR           => register_addr_buf,
    REGISTER_WRITE_OR_READ  => register_write_or_read_buf,
    REGISTER_DATA           => register_data_buf,

    ETH_IP        => eth_ip,
    ETH_MAC       => eth_mac,

    UDP_PKG_CTR   => udp_pkg_ctr
  );

  -- control signals for the lcd
  SF_D <= LCD_DB;
  LCD_E <= LCD_CTRL(0);
  LCD_RW <= LCD_CTRL(1);
  LCD_RS <= LCD_CTRL(2);


  -- a command for the register came in, display it on the LCD
  display_register_set : process ( gtx_clk_bufg, GLBL_RST, register_access, register_access_ready, register_display_counter_running, register_write_or_read, eth_switch_change, eth_display_counter_running )
  begin
    if ( register_write_or_read = '1' ) then
      register_display_counter_enable <= register_display_counter_running or register_access;
    else
      register_display_counter_enable <= register_display_counter_running or register_access_ready;
    end if;

    -- a global reset came in
    if ( GLBL_RST = '1' ) then
      lcd_mode <= lcd_mode_default;
      register_display_counter <= 0;
      register_display_counter_running <= '0';

      eth_display_counter <= 0;
      eth_display_counter_running <= '0';
      eth_switch_buffer <= eth_switch;

      register_addr_buf <= (others => '0');
      register_write_or_read_buf <= '0';
      register_data_buf <= (others => '0');

    elsif rising_edge( gtx_clk_bufg ) then

      -- the display of the register command is active
      if ( lcd_enable_register_display = '1' and register_display_counter_enable = '1' ) then
        lcd_mode <= "011";

        if ( register_display_counter = register_display_counter_max ) then
          register_display_counter <= 0;
          register_display_counter_running <= '0';
        else
          if ( register_display_counter = 0 ) then
            register_addr_buf <= register_addr(11 downto 0); --sregs_regaddr (9 downto 2);--register_addr;
            register_write_or_read_buf <= register_write_or_read;
            if ( register_write_or_read = '1' ) then
              register_data_buf <= register_write_data;
            else
              register_data_buf <= register_read_data;
            end if;
          end if;

          register_display_counter_running <= '1';
          register_display_counter <= register_display_counter + 1;
        end if;

      -- the ethernet configuration has changed, display it
      elsif ( lcd_enable_eth_display = '1' and ( eth_switch_change = '1' or eth_display_counter_running = '1' ) ) then
        lcd_mode <= "100";

        if ( eth_switch_change = '1' ) then
          eth_switch_buffer <= eth_switch;
          eth_display_counter <= 0;
          eth_display_counter_running <= '1';
        elsif ( eth_display_counter = eth_display_counter_max ) then
          eth_switch_buffer <= eth_switch_buffer;
          eth_display_counter <= 0;
          eth_display_counter_running <= '0';
        else
          eth_switch_buffer <= eth_switch_buffer;
          eth_display_counter <= eth_display_counter + 1;
          eth_display_counter_running <= '1';
        end if;

      -- fall back to displaying the default
      else
        lcd_mode <= lcd_mode_default;
      end if;
    end if;

  end process;


  ------------------------------------------------------------------------------
  -- Ethernet wrapper
  ------------------------------------------------------------------------------

  eth_switch <= USER_SWITCH(3 downto 0);
  eth_switch_change <=
    ( eth_switch(0) xor eth_switch_buffer(0) ) or
    ( eth_switch(1) xor eth_switch_buffer(1) ) or
    ( eth_switch(2) xor eth_switch_buffer(2) ) or
    ( eth_switch(3) xor eth_switch_buffer(3) );

  ETH_WRAPPER : entity work.ethernet_core_wrapper
  port map (

    -- asynchronous reset
    GLBL_RST      => GLBL_RST,

    -- input clocks from generator
    REFCLK_BUFG   => refclk_bufg,
    GTX_CLK_BUFG  => gtx_clk_bufg,
    DCM_LOCKED    => clk_locked,

    PHY_RESETN    => PHY_RESETN,

    -- GMII Interface
    -----------------
    GMII_TXD      => GMII_TXD,
    GMII_TX_EN    => GMII_TX_EN,
    GMII_TX_ER    => GMII_TX_ER,
    GMII_TX_CLK   => GMII_TX_CLK,
    GMII_RXD      => GMII_RXD,
    GMII_RX_DV    => GMII_RX_DV,
    GMII_RX_ER    => GMII_RX_ER,
    GMII_RX_CLK   => GMII_RX_CLK,
    GMII_COL      => GMII_COL,
    GMII_CRS      => GMII_CRS,
    MII_TX_CLK    => MII_TX_CLK,

    -- Main example design controls
    -------------------------------
    DISPLAY         => open, --USER_LED,
    USER_SWITCH     => eth_switch,
    ETH_IP          => eth_ip,
    ETH_MAC         => eth_mac,
    RX_PKG_CTR      => udp_pkg_ctr,

    REGISTER_ACCESS         => register_access,
    REGISTER_ADDR           => register_addr,
    REGISTER_READ_READY     => register_access_ready,
    REGISTER_WRITE_OR_READ  => register_write_or_read,
    REGISTER_READ_DATA      => register_read_data,
    REGISTER_WRITE_DATA     => register_write_data,

    REGISTER_BLK_EN         => register_blk_en,
    REGISTER_BLK_RDEN       => register_blk_rden,
    REGISTER_BLK_DATA       => register_blk_data,
    REGISTER_BLK_COUNT      => register_blk_count,
    REGISTER_BLK_EMPTY      => register_blk_empty,
    REGISTER_BLK_VALID      => register_blk_valid
  );




  ------------------------------------------------------------------------------
  -- Register control
  ------------------------------------------------------------------------------

  REG_CTRL : entity work.RegisterControl
  port map (
    CLK             => gtx_clk_bufg,
    RESET           => GLBL_RST,
    LED             => USER_LED,

    -- spill control
    SPILL_ACTIVE    => SMA_GPIO_N,
    PUSHBTN_SW      => PUSHBTN_SW(0),
    PUSHBTN_LED     => PUSHBTN_LED(0),

    -- MMCM output
    CLK_MMCM        => open,
    RST_MMCM        => open,

    -- register handling
    REG_EN          => register_access,
    REG_WR          => register_write_or_read,
    REG_ADDR        => register_addr(11 downto 0),
    REG_DATA        => register_write_data,
    REG_DATA_OUT    => register_read_data,
    REG_VALID       => register_access_ready,

    -- generic registers
    REG_DEV0_CONFIG => reg_dev0_config,
    REG_DEV0_CFG_WR => reg_dev0_cfg_wr,
    REG_DEV0_STATUS => reg_dev0_status,
    REG_DEV0_ST_RD  => reg_dev0_st_rd,
    REG_DEV1_CONFIG => reg_dev1_config,
    REG_DEV1_CFG_WR => reg_dev1_cfg_wr,
    REG_DEV1_STATUS => reg_dev1_status,
    REG_DEV1_ST_RD  => reg_dev1_st_rd,

    -- bulk transfer
    REG_BLK_EN      => register_blk_en,
    REG_BLK_RDEN    => register_blk_rden,
    REG_BLK_DOUT    => register_blk_data,
    REG_BLK_COUNT   => register_blk_count,
    REG_BLK_EMPTY   => register_blk_empty,
    REG_BLK_VALID   => register_blk_valid,
    BLK_DEV0_WREN   => '0',
    BLK_DEV0_DIN    => (others => '0')
  );


end Behavioral;
