----------------------------------------------------------------------------------
-- Company:   IKP-1, FZ Jlich
-- Engineer:  Andre Goerres
--
-- Create Date:     09-Apr-2014
-- Design Name:     ML605 Ethernet UDP
-- Module Name:     Ethernet testbench
-- Project Name:
-- Target Devices:
-- Tool versions:
-- Description: This testbench is designed to take the top level description
--              of the ML605 core, including the Ethernet and UDP blocks.
--              It initializes a connection with the core and interchanges
--              some data with it.
--
-- Dependencies:    All modules from the ../vhdl/ folder.
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



entity ml605_ethernet_tb is
end ml605_ethernet_tb;


architecture behavioral of ml605_ethernet_tb is

  ------------------------------------------------------------------------------
  -- toplevel port map signals
  ------------------------------------------------------------------------------
  -- asynchronous reset
  signal GLBL_RST       : std_logic; -- in

  -- 200MHz clock input from board
  signal CLK_IN_P       : std_logic; -- in
  signal CLK_IN_N       : std_logic; -- in

  -- 66MHz clock input from board
  signal CLK66          : std_logic; -- in

  signal PHY_RESETN     : std_logic; -- out
  signal SM_FAN_PWM     : std_logic; -- out

  -- LCD  interface
  ------------------
  signal SF_D           : std_logic_vector(3 downto 0); -- out  -- LCD data bus
  signal LCD_E          : std_logic; -- out             -- LCD: E   (control bit)
  signal LCD_RS         : std_logic; -- out             -- LCD: RS  (setup or data)
  signal LCD_RW         : std_logic; -- out             -- LCD: R/W (read or write)

  -- GMII Interface
  -----------------

  signal GMII_TXD       : std_logic_vector(7 downto 0); -- out
  signal GMII_TX_EN     : std_logic; -- out
  signal GMII_TX_ER     : std_logic; -- out
  signal GMII_TX_CLK    : std_logic; -- out
  signal GMII_RXD       : std_logic_vector(7 downto 0); -- in
  signal GMII_RX_DV     : std_logic; -- in
  signal GMII_RX_ER     : std_logic; -- in
  signal GMII_RX_CLK    : std_logic; -- in
  signal GMII_COL       : std_logic; -- in
  signal GMII_CRS       : std_logic; -- in
  signal MII_TX_CLK     : std_logic; -- in

  -- Serialised statistics vectors
  --------------------------------
  signal TX_STATISTICS_S  : std_logic; -- out
  signal RX_STATISTICS_S  : std_logic; -- out

  -- Serialised Pause interface controls
  --------------------------------------
  signal PAUSE_REQ_S    : std_logic; -- in

  -- Design controls and output
  -------------------------------
  signal USER_LED       : std_logic_vector (7 downto 0); -- out
  signal USER_SWITCH    : std_logic_vector (4 downto 0); -- in

  ------------------------------------------------------------------------------
  -- internal signals
  ------------------------------------------------------------------------------


begin

  ml605_topl : entity work.topl
  port map (
    -- asynchronous reset
    GLBL_RST            => GLBL_RST,

    -- 200MHz clock input from board
    CLK_IN_P            => CLK_IN_P,
    CLK_IN_N            => CLK_IN_N,

    -- 66MHz clock input from board
    CLK66               => CLK66,

    -- 125MHz GTX clock
    -- MGTREFCLK_N         => MGTREFCLK_N,
    -- MGTREFCLK_P         => MGTREFCLK_P,

    PHY_RESETN          => PHY_RESETN,
    SM_FAN_PWM          => SM_FAN_PWM,

    -- LCD  interface
    ------------------
    SF_D                => SF_D,    -- LCD data bus
    LCD_E               => LCD_E,   -- LCD: E   (control bit)
    LCD_RS              => LCD_RS,  -- LCD: RS  (setup or data)
    LCD_RW              => LCD_RW,  -- LCD: R/W (read or write)

    -- GMII Interface
    -----------------

    GMII_TXD            => GMII_TXD,
    GMII_TX_EN          => GMII_TX_EN,
    GMII_TX_ER          => GMII_TX_ER,
    GMII_TX_CLK         => GMII_TX_CLK,
    GMII_RXD            => GMII_RXD,
    GMII_RX_DV          => GMII_RX_DV,
    GMII_RX_ER          => GMII_RX_ER,
    GMII_RX_CLK         => GMII_RX_CLK,
    GMII_COL            => GMII_COL,
    GMII_CRS            => GMII_CRS,
    MII_TX_CLK          => MII_TX_CLK,

    -- Serialised statistics vectors
    --------------------------------
    TX_STATISTICS_S     => TX_STATISTICS_S,
    RX_STATISTICS_S     => RX_STATISTICS_S,

    -- Serialised Pause interface controls
    --------------------------------------
    PAUSE_REQ_S         => PAUSE_REQ_S,

    -- Design controls and output
    -------------------------------
    USER_LED            => USER_LED,
    USER_SWITCH         => USER_SWITCH
  );

  -- default input signals
  USER_SWITCH <= "00000";

end behavioral;
