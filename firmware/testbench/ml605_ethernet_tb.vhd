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

  constant bytes_MAC        : integer := 6;
  constant bytes_IP         : integer := 4;


  -- IP & MAC addresses
  type IP_address  is array (0 to bytes_IP-1) of byte;
  type MAC_address is array (0 to bytes_MAC-1) of byte;
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

  -- input data signals
  signal mac_tx_tready          : std_logic;
  signal mac_rx_tvalid          : std_logic;
  signal mac_rx_tdata           : std_logic_vector(7 downto 0);
  signal mac_rx_tlast           : std_logic;

  -- mirrored UDP/IP signals
  signal mac_tx_tvalid          : std_logic;
  signal mac_tx_tlast           : std_logic;
  signal udp_rx                 : udp_rx_type;
  signal udp_rx_start           : std_logic;
  signal udp_tx_result          : std_logic_vector (1 downto 0);
  signal udp_tx_data_out_ready  : std_logic;
  signal arp_pkt_count          : std_logic_vector(7 downto 0);
  signal ip_rx_hdr              : ipv4_rx_header_type;
  signal ip_pkt_count           : std_logic_vector(7 downto 0);


  -- Delay to provide setup and hold timing at the GMII/MII.
  constant dly : time := 2 ns;


  -- The signal_force procedure needs a string as input, so we need to convert
  -- the std_logic_vector to a string.
  function slv_to_string ( slv : in std_logic_vector )
  return string is
    variable result : string ( 1 to slv'length );
  begin
    for i in slv'range loop
      case slv(slv'length - i - 1) is
        when '0' => result(i+1) := '0';
        when '1' => result(i+1) := '1';
        when others => result(i+1) := '0';
      end case;
    end loop;
    return result;
  end slv_to_string;


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

    -- just spy on these
    init_signal_spy("ml605_topl/eth_wrapper/tx_axis_fifo_tvalid",       "mac_tx_tvalid");
    init_signal_spy("ml605_topl/eth_wrapper/tx_axis_fifo_tlast",        "mac_tx_tlast");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/udp_tx_result",   "udp_tx_result");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/udp_tx_data_out_ready", "udp_tx_data_out_ready");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/arp_pkt_count",   "arp_pkt_count");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/ip_pkt_count",    "ip_pkt_count");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/udp_rx_start",    "udp_rx_start");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/udp_rxo",         "udp_rx");
    init_signal_spy("ml605_topl/eth_wrapper/udp_block/ip_rx_hdr",       "ip_rx_hdr");
    wait;
  end process;

  -- modify these signals (skip the ethernet core from the evaluation board)
  force_mac_tx_tready : process (mac_tx_tready)
  begin
    if mac_tx_tready = '1' then
      signal_force("ml605_topl/eth_wrapper/tx_axis_fifo_tready", "1", 0 ps, freeze, open, 0);
    else
      signal_force("ml605_topl/eth_wrapper/tx_axis_fifo_tready", "0", 0 ps, freeze, open, 0);
    end if;
  end process;
  force_mac_rx_tvalid : process (mac_rx_tvalid)
  begin
    if mac_rx_tvalid = '1' then
      signal_force("ml605_topl/eth_wrapper/rx_axis_fifo_tvalid", "1", 0 ps, freeze, open, 0);
    else
      signal_force("ml605_topl/eth_wrapper/rx_axis_fifo_tvalid", "0", 0 ps, freeze, open, 0);
    end if;
  end process;
  force_mac_rx_tdata : process (mac_rx_tdata)
  begin
    signal_force("ml605_topl/eth_wrapper/rx_axis_fifo_tdata", slv_to_string(mac_rx_tdata), 0 ps, freeze, open, 0);
  end process;
  force_mac_rx_tlast : process (mac_rx_tlast)
  begin
    if mac_rx_tlast = '1' then
      signal_force("ml605_topl/eth_wrapper/rx_axis_fifo_tlast", "1", 0 ps, freeze, open, 0);
    else
      signal_force("ml605_topl/eth_wrapper/rx_axis_fifo_tlast", "0", 0 ps, freeze, open, 0);
    end if;
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
    CLK66 <= '1';
    wait for clk66_period/2;
    CLK66 <= '0';
    wait for clk66_period/2;
  end process;

  clk125_process : process
  begin
    GMII_RX_CLK <= '1';
    wait for clk125_period/2;
    GMII_RX_CLK <= '0';
    wait for clk125_period/2;
  end process;


  ------------------------------------------------------------------------------
  --  send data ...
  ------------------------------------------------------------------------------

  send_data : process
  begin
    -- For some strange reason, the UDP stuff gets initialized after 100 ns.
    wait for 10*clk125_period;

    -- power-on reset
    GLBL_RST <= '1', '0' after 2*clk125_period;
    mac_tx_tready <= '0';
    mac_rx_tvalid <= '0';
    mac_rx_tlast  <= '0';
    mac_rx_tdata  <= (others => '0');
    wait for 30*clk125_period;  -- wait for the MMCM to release the clock lock

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


    mac_tx_tready <= '1';

    ------------
    -- TEST 1 -- send ARP request
    ------------

    report "T1: Send an ARP request: who has 192.168.0.10? Tell 192.168.0.1";

    mac_rx_tvalid <= '1';

    -- dst MAC (broadcast)
    mac_rx_tdata <= x"FF"; wait for bytes_MAC*clk125_period;
    -- src MAC
    for i in our_MAC_address'range loop
      mac_rx_tdata <= our_MAC_address(i); wait for clk125_period;
    end loop;
    -- type
    mac_rx_tdata <= x"08"; wait for clk125_period;    -- ARP pkt
    mac_rx_tdata <= x"06"; wait for clk125_period;
    -- HW type
    mac_rx_tdata <= x"00"; wait for clk125_period;
    mac_rx_tdata <= x"01"; wait for clk125_period;
    -- Protocol type
    mac_rx_tdata <= x"08"; wait for clk125_period;
    mac_rx_tdata <= x"00"; wait for clk125_period;
    -- HW size
    mac_rx_tdata <= x"06"; wait for clk125_period;
    -- protocol size
    mac_rx_tdata <= x"04"; wait for clk125_period;
    -- Opcode
    mac_rx_tdata <= x"00"; wait for clk125_period;
    mac_rx_tdata <= x"01"; wait for clk125_period;
    -- Sender MAC
    for i in our_MAC_address'range loop
      mac_rx_tdata <= our_MAC_address(i); wait for clk125_period;
    end loop;
    -- Sender IP
    for i in our_IP_address'range loop
      mac_rx_tdata <= our_IP_address(i); wait for clk125_period;
    end loop;
    -- Target MAC
    mac_rx_tdata <= x"00"; wait for bytes_MAC*clk125_period;
    -- Target IP
    for i in FPGA_IP_address'range loop
      mac_rx_tdata <= FPGA_IP_address(i); wait for clk125_period;
    end loop;
    -- filler
    mac_rx_tdata <= x"00"; wait for 3*clk125_period;
    mac_rx_tlast <= '1';
    mac_rx_tdata <= x"00"; wait for clk_period;
    mac_rx_tlast <= '0';
    mac_rx_tvalid <= '0';


    -- check if we got the ARP pkt
    assert arp_pkt_count = x"01"              report "T1: arp_pkt_count wrong value";
    assert ip_pkt_count = x"00"               report "T1: ip_pkt_count wrong value";
    assert udp_tx_result = UDPTX_RESULT_NONE  report "T1: udp_tx_result wrong value";
    assert udp_tx_data_out_ready = '0'        report "T1: ip_udp_txitx.data.data_out_ready wrong value";
    assert udp_rx_start = '0'                 report "T1: udp_rx_start wrong value";
    assert udp_rx.hdr.is_valid = '0'          report "T1: udp_rx.hdr.is_valid wrong value";
    assert ip_rx_hdr.is_valid = '0'           report "T1: ip_rx_hdr.is_valid wrong value";


    -- check if we tx a response
    wait for clk125_period*25;
    assert mac_tx_tvalid = '1'                report "T1: not transmitting a response";
    wait for clk125_period*25;
    assert mac_tx_tvalid = '0'                report "T1: tx held on for too long";


    report "T1: test complete!";


    -- ------------
    -- -- TEST 2 -- send ARP request
    -- ------------

    -- report "T2: Send an ARP request: who has 192.168.0.10? Tell 192.168.0.1";

    -- -- Ethernet package initalization
    -- -- GMII_CRS    <= '1';   wait for 3*clk125_period;
    -- GMII_RX_DV  <= '1' after dly;
    -- GMII_RXD    <= x"55" after dly; wait for 7*clk125_period;
    -- GMII_RXD    <= x"D5" after dly; wait for 1*clk125_period;

    -- -- Ethernet package

    -- -- dst MAC
    -- for i in FPGA_MAC_address'range loop
    --   GMII_RXD <= FPGA_MAC_address(i) after dly; wait for clk125_period;
    -- end loop;
    -- -- src MAC
    -- for i in our_MAC_address'range loop
    --   GMII_RXD <= our_MAC_address(i) after dly; wait for clk125_period;
    -- end loop;

    -- -- type
    -- GMII_RXD <= x"08" after dly; wait for clk125_period;    -- IP pkt
    -- GMII_RXD <= x"00" after dly; wait for clk125_period;
    -- -- ver & HL / service type
    -- GMII_RXD <= x"45" after dly; wait for clk125_period;
    -- GMII_RXD <= x"00" after dly; wait for clk125_period;
    -- -- total len
    -- GMII_RXD <= x"00" after dly; wait for clk125_period;
    -- GMII_RXD <= x"21" after dly; wait for clk125_period;
    -- -- ID
    -- GMII_RXD <= x"00" after dly; wait for clk125_period;
    -- GMII_RXD <= x"7a" after dly; wait for clk125_period;
    -- -- flags & frag
    -- GMII_RXD <= x"00" after dly; wait for clk125_period;
    -- GMII_RXD <= x"00" after dly; wait for clk125_period;
    -- -- TTL
    -- GMII_RXD <= x"80" after dly; wait for clk125_period;
    -- -- Protocol
    -- GMII_RXD <= x"11" after dly; wait for clk125_period;
    -- -- Header CKS
    -- GMII_RXD <= x"00" after dly; wait for clk125_period;
    -- GMII_RXD <= x"00" after dly; wait for clk125_period;

    -- -- Sender IP
    -- for i in our_IP_address'range loop
    --   GMII_RXD <= our_IP_address(i) after dly; wait for clk125_period;
    -- end loop;
    -- -- Target IP
    -- for i in FPGA_IP_address'range loop
    --   GMII_RXD <= FPGA_IP_address(i) after dly; wait for clk125_period;
    -- end loop;
    -- -- SRC port
    -- GMII_RXD <= x"f4" after dly; wait for clk125_period;
    -- GMII_RXD <= x"9a" after dly; wait for clk125_period;
    -- -- DST port
    -- GMII_RXD <= x"26" after dly; wait for clk125_period;
    -- GMII_RXD <= x"94" after dly; wait for clk125_period;
    -- -- cks
    -- GMII_RXD <= x"8b" after dly; wait for clk125_period;
    -- GMII_RXD <= x"79" after dly; wait for clk125_period;
    -- -- user data
    -- GMII_RXD <= x"68" after dly; wait for clk125_period;

    -- -- put the rest of the user data
    -- GMII_RXD <= x"65" after dly; wait for clk125_period;
    -- GMII_RXD <= x"6c" after dly; wait for clk125_period;
    -- GMII_RXD <= x"6c" after dly; wait for clk125_period;
    -- GMII_RXD <= x"6f" after dly; wait for clk125_period;

    --     -- Ethernet package trailer (skip FCS and send x"AA" instead)
    -- GMII_RXD    <= x"AA" after dly; wait for 3*clk125_period;
    -- -- GMII_CRS    <= '0';   wait for 1*clk125_period;
    -- GMII_RXD    <= x"00" after dly;
    -- GMII_RX_DV  <= '0' after dly;

    -- finish
    wait for 100 ms;
    assert false report "  Simulation finished." severity failure;
  end process;

end behavioral;
