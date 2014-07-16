--------------------------------------------------------------------------------
-- Authors:       André Goerres (FZJ) - a.goerres@fz-juelich.de
--
-- Create Date:   2014-04-09
-- Design Name:   ML605 Ethernet UDP
-- Project Name:  Ethernet testbench
-- Description:   This testbench is designed to take the top level description
--                of the ML605 core, including the Ethernet and UDP blocks.
--                It initializes a connection with the core and interchanges
--                some data with it.
--
-- Dependencies:  TDC_CTRL_top, grayclk
--
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
--------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
library UNISIM;
use UNISIM.VComponents.all;

-- Use modelsim specific functions to test for correct behavior
library modelsim_lib;
use modelsim_lib.util.all;

-- constants and variable types
use work.axi.all;
use work.ipv4_types.all;
use work.arp_types.all;


entity ml605_ethernet_tb is
end ml605_ethernet_tb;


architecture behavioral of ml605_ethernet_tb is

  subtype byte is std_logic_vector(7 downto 0);

  -- some constants
  constant clk_period     : time := 5 ns; -- master 200 MHz clock
  constant clk66_period   : time := 15.151515 ns; -- 66 MHz clock
  constant clk125_period  : time := 8 ns; -- 125 MHz Ethernet clock


  -- IP & MAC addresses
  type IP_address  is array (0 to 3) of byte;
  type MAC_address is array (0 to 5) of byte;
  constant our_IP_address   : IP_address  := (x"C0", x"A8", x"00", x"01");
  constant FPGA_IP_address  : IP_address  := (x"C0", x"A8", x"00", x"0A");
  constant our_MAC_address  : MAC_address := (x"54", x"04", x"A6", x"69", x"15", x"26");
  constant FPGA_MAC_address : MAC_address := (x"00", x"0A", x"35", x"02", x"2E", x"62");


  ------------------------------------------------------------------------------
  -- toplevel port map signals
  ------------------------------------------------------------------------------
  -- asynchronous reset
  signal GLBL_RST       : std_logic := '1'; -- in

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
  signal GMII_RXD       : std_logic_vector(7 downto 0) := (others => '0'); -- in
  signal GMII_RX_DV     : std_logic := '0'; -- in
  signal GMII_RX_ER     : std_logic := '0'; -- in
  signal GMII_RX_CLK    : std_logic := '0'; -- in
  signal GMII_COL       : std_logic := '0'; -- in
  signal GMII_CRS       : std_logic := '0'; -- in
  signal MII_TX_CLK     : std_logic := '0'; -- in

  -- Serialised statistics vectors
  --------------------------------
  signal TX_STATISTICS_S  : std_logic; -- out
  signal RX_STATISTICS_S  : std_logic; -- out

  -- Serialised Pause interface controls
  --------------------------------------
  signal PAUSE_REQ_S    : std_logic := '0'; -- in

  -- Design controls and output
  -------------------------------
  signal USER_LED       : std_logic_vector (7 downto 0); -- out
  signal USER_SWITCH    : std_logic_vector (4 downto 0) := "00000"; -- in


  ------------------------------------------------------------------------------
  -- internal signals
  ------------------------------------------------------------------------------

  -- mirrored UDP/IP signals
  signal udp_rx                 : udp_rx_type;
  signal udp_rx_start           : std_logic;
  signal udp_tx_result          : std_logic_vector (1 downto 0);
  signal udp_tx_data_out_ready  : std_logic;
  signal arp_pkt_count          : std_logic_vector(7 downto 0);
  signal ip_rx_hdr              : ipv4_rx_header_type;
  signal ip_pkt_count           : std_logic_vector(7 downto 0);
  -- signal mac_tx_tdata           : std_logic_vector(7 downto 0);
  signal mac_tx_tvalid          : std_logic;
  -- signal mac_tx_tfirst          : std_logic;
  signal mac_tx_tlast           : std_logic;
  -- signal mac_rx_tready          : std_logic;


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

  -- mirror UDP/IP signals
  mirrors : process
  begin
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/udp_tx_result", "udp_tx_result");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/udp_tx_data_out_ready", "udp_tx_data_out_ready");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/mac_tx_tvalid", "mac_tx_tvalid");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/mac_tx_tlast", "mac_tx_tlast");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/arp_pkt_count", "arp_pkt_count");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/ip_pkt_count", "ip_pkt_count");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/udp_rx_start", "udp_rx_start");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/udp_rxo", "udp_rx");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/ip_rx_hdr", "ip_rx_hdr");
    wait;
  end process;


  clk_process : process
  begin
    CLK_IN_P <= '0';
    CLK_IN_N <= '1';
    wait for clk_period/2;
    CLK_IN_P <= '1';
    CLK_IN_N <= '0';
    wait for clk_period/2;
  end process;

  clk66_process : process
  begin
    CLK66 <= '0';
    wait for clk66_period/2;
    CLK66 <= '1';
    wait for clk66_period/2;
  end process;

  clk125_process : process
  begin
    GMII_RX_CLK <= '0';
    wait for clk125_period/2;
    GMII_RX_CLK <= '1';
    wait for clk125_period/2;
  end process;


  ------------------------------------------------------------------------------
  --  send data ...
  ------------------------------------------------------------------------------

  send_data : process
  begin
    -- For some strange reason, the UDP stuff gets initialized after 100 ns.
    wait for 100 ns;

    -- power-on reset
    GLBL_RST <= '1', '0' after 2*clk125_period;
    wait for 10*clk125_period;

    -- check reset conditions
    assert udp_tx_result = UDPTX_RESULT_NONE    report "udp_tx_result not initialised correctly on reset";
    assert udp_tx_data_out_ready = '0'          report "ip_udp_txitx.data.data_out_ready not initialised correctly on reset";
    assert mac_tx_tvalid = '0'                  report "mac_tx_tvalid not initialised correctly on reset";
    assert mac_tx_tlast = '0'                   report "mac_tx_tlast not initialised correctly on reset";
    assert arp_pkt_count = x"00"                report "arp_pkt_count not initialised correctly on reset";
    assert ip_pkt_count = x"00"                 report "ip_pkt_count not initialised correctly on reset";
    assert udp_rx_start = '0'                   report "udp_rx_start not initialised correctly on reset";
    assert udp_rx.hdr.is_valid = '0'            report "udp_rx.hdr.is_valid not initialised correctly on reset";
    assert udp_rx.hdr.data_length = x"0000"     report "udp_rx.hdr.data_length not initialised correctly on reset";
    assert udp_rx.hdr.src_ip_addr = x"00000000" report "udp_rx.hdr.src_ip_addr not initialised correctly on reset";
    assert udp_rx.hdr.src_port = x"0000"        report "udp_rx.hdr.src_port not initialised correctly on reset";
    assert udp_rx.hdr.dst_port = x"0000"        report "udp_rx.hdr.dst_port not initialised correctly on reset";
    assert udp_rx.data.data_in = x"00"          report "udp_rx.data.data_in not initialised correctly on reset";
    assert udp_rx.data.data_in_valid = '0'      report "udp_rx.data.data_in_valid not initialised correctly on reset";
    assert udp_rx.data.data_in_last = '0'       report "udp_rx.data.data_in_last not initialised correctly on reset";
    assert ip_rx_hdr.is_valid = '0'             report "ip_rx_hdr.is_valid not initialised correctly on reset";
    assert ip_rx_hdr.protocol = x"00"           report "ip_rx_hdr.protocol not initialised correctly on reset";
    assert ip_rx_hdr.data_length = x"0000"      report "ip_rx_hdr.data_length not initialised correctly on reset";
    assert ip_rx_hdr.src_ip_addr = x"00000000"  report "ip_rx_hdr.src_ip_addr not initialised correctly on reset";
    assert ip_rx_hdr.num_frame_errors = x"00"   report "ip_rx_hdr.num_frame_errors not initialised correctly on reset";



    ------------
    -- TEST 1 -- send ARP request
    ------------

    report "T1: Send an ARP request: who has 192.168.5.9? Tell 192.168.5.1";

    -- Ethernet package initalization
    GMII_CRS    <= '1';   wait for 3*clk125_period;
    GMII_RX_DV  <= '1';
    GMII_RXD    <= x"55"; wait for 7*clk125_period;
    GMII_RXD    <= x"D5"; wait for 1*clk125_period;

    -- Ethernet package
    GMII_RXD <= x"05"; wait for 10*clk125_period;

    -- Ethernet package trailer (skip FCS and send x"AA" instead)
    GMII_RXD    <= x"AA"; wait for 3*clk125_period;
    GMII_CRS    <= '0';   wait for 1*clk125_period;
    GMII_RXD    <= x"00";
    GMII_RX_DV  <= '0';




    -- finish
    wait for 100 ms;
    assert false report "  Simulation finished." severity failure;
  end process;

end behavioral;
