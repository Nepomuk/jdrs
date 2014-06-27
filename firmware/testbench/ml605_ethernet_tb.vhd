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

  -- what_to_do is the first byte of the UDP data payload and is
  --  an identifyer, that tells us what should be done.
  --  (copied from ethernet_core_wrapper.vhd)
  subtype what_to_do_type is std_logic_vector(7 downto 0);
  constant do_nothing         : what_to_do_type := x"00";  -- nothing is happening here, just keep going
  constant do_ping            : what_to_do_type := x"01";  -- request a pong reply
  constant do_pong            : what_to_do_type := x"02";  -- answer to a ping request
  constant do_confirm         : what_to_do_type := x"03";  -- confirmation
  constant do_error           : what_to_do_type := x"04";  -- something went wrong
  constant do_timeout         : what_to_do_type := x"05";  -- a timeout for a request occured
  constant do_read_register   : what_to_do_type := x"11";  -- read out a register value
  constant do_write_register  : what_to_do_type := x"12";  -- set a register value
  constant do_pkg_count_read  : what_to_do_type := x"13";  -- read out the package counter
  constant do_pkg_count_reset : what_to_do_type := x"14";  -- reset the package counter
  constant do_read_dma        : what_to_do_type := x"21";  -- read from the DMA fifo


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

  -- fix behavior of simulation
  type UDP_state_type is (IDLE, WAIT_RX_DONE, WAIT_DATA_OUT_RDY, DATA_OUT, FINISH_SEND);
  type count_mode_type is (RST, INCR, HOLD);
  signal UDP_state, UDP_next_state : UDP_state_type;
  signal UDP_set_state          : std_logic;
  signal UDP_set_tx_count       : count_mode_type;


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

    -- some more to fix strange behavior in simulation
    init_signal_spy("ml605_topl/eth_wrapper/set_state",   "UDP_set_state");
    init_signal_spy("ml605_topl/eth_wrapper/state",       "UDP_state");
    init_signal_spy("ml605_topl/eth_wrapper/next_state",  "UDP_next_state");
    init_signal_spy("ml605_topl/eth_wrapper/set_tx_count","UDP_set_tx_count");

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


  -- We need to fix a strange behavior of the simulation: for some reason it
  -- doesn't change the state of the UDP wrapper correctly.
  process
  begin
    wait until UDP_set_state = '1';

    signal_force("ml605_topl/eth_wrapper/set_state", "1", 0 ps, freeze, clk125_period - 10 ps, 0);

    case UDP_next_state is
      when IDLE =>
        signal_force("ml605_topl/eth_wrapper/state", "IDLE", 0 ps, deposit, open, 0);
      when WAIT_RX_DONE =>
        signal_force("ml605_topl/eth_wrapper/state", "WAIT_RX_DONE", 0 ps, deposit, open, 0);
      when WAIT_DATA_OUT_RDY =>
        signal_force("ml605_topl/eth_wrapper/state", "WAIT_DATA_OUT_RDY", 0 ps, deposit, open, 0);
      when DATA_OUT =>
        signal_force("ml605_topl/eth_wrapper/state", "DATA_OUT", 0 ps, deposit, open, 0);
      when FINISH_SEND =>
        signal_force("ml605_topl/eth_wrapper/state", "FINISH_SEND", 0 ps, deposit, open, 0);
    end case;

    wait for clk125_period;
  end process;

  -- sending of a package is also strange, the counter starts too early
  process
  begin
    wait until UDP_set_tx_count = INCR;
    signal_force("ml605_topl/eth_wrapper/tx_count", "16#0000", 0 ps, deposit, open, 1);
    wait until UDP_set_tx_count = RST;
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
    mac_rx_tdata <= x"00"; wait for clk125_period;
    mac_rx_tvalid <= '0';
    mac_rx_tlast <= '0';


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
    -- -- TEST 2 -- send UDP pkt (ping)
    -- ------------

    report "T2: Send an UDP package: ping";

    mac_rx_tvalid <= '1';

    ------- ETH ----------
    -- dst MAC (broadcast)
    for i in FPGA_MAC_address'range loop
      mac_rx_tdata <= FPGA_MAC_address(i); wait for clk125_period;
    end loop;
    -- src MAC
    for i in our_MAC_address'range loop
      mac_rx_tdata <= our_MAC_address(i); wait for clk125_period;
    end loop;
    -- type
    mac_rx_tdata <= x"08"; wait for clk125_period;    -- IP pkt
    mac_rx_tdata <= x"00"; wait for clk125_period;

    ------- IPIP ----------
    -- version & HL (header length)
    mac_rx_tdata <= x"45"; wait for clk125_period;
    -- service type
    mac_rx_tdata <= x"00"; wait for clk125_period;
    -- total len
    mac_rx_tdata <= x"00"; wait for clk125_period;
    mac_rx_tdata <= x"21"; wait for clk125_period;
    -- ID
    mac_rx_tdata <= x"00"; wait for clk125_period;
    mac_rx_tdata <= x"7a"; wait for clk125_period;
    -- flags & frag
    mac_rx_tdata <= x"00"; wait for clk125_period;
    mac_rx_tdata <= x"00"; wait for clk125_period;
    -- TTL
    mac_rx_tdata <= x"80"; wait for clk125_period;
    -- Protocol
    mac_rx_tdata <= x"11"; wait for clk125_period;
    -- Header CKS
    mac_rx_tdata <= x"00"; wait for clk125_period; -- skip the checksum
    mac_rx_tdata <= x"00"; wait for clk125_period;
    -- src IP
    for i in our_IP_address'range loop
      mac_rx_tdata <= our_IP_address(i); wait for clk125_period;
    end loop;
    -- dst IP
    for i in FPGA_IP_address'range loop
      mac_rx_tdata <= FPGA_IP_address(i); wait for clk125_period;
    end loop;

    ------- UDP ----------
    -- src port
    mac_rx_tdata <= x"C3"; wait for clk125_period; -- port 50,000
    mac_rx_tdata <= x"50"; wait for clk125_period;
    -- dst port
    mac_rx_tdata <= x"C3"; wait for clk125_period; -- port 50,000
    mac_rx_tdata <= x"50"; wait for clk125_period;
    -- length
    mac_rx_tdata <= x"00"; wait for clk125_period;
    mac_rx_tdata <= x"0c"; wait for clk125_period; -- header (8 byte + data)
    -- cks
    mac_rx_tdata <= x"00"; wait for clk125_period; -- skip the checksum
    --mac_rx_tdata <= x"00"; wait for clk125_period;  (for some reason, the simulation has an offset by one clk cycle, the real thing works though)

    -- user data
    mac_rx_tdata <= do_ping; wait for clk125_period;

    -- since we are up to the user data stage, the header should be valid and the data_in_valid should be set
    assert udp_rx.hdr.is_valid = '1'                report "T2: udp_rx.hdr.is_valid not set";
    assert udp_rx.hdr.data_length = x"0004"         report "T2: udp_rx.hdr.data_length not set correctly";
    assert udp_rx.hdr.src_ip_addr = x"c0a80001"     report "T2: udp_rx.hdr.src_ip_addr not set correctly";
    assert udp_rx.hdr.src_port = x"C350"            report "T2: udp_rx.hdr.src_port not set correctly";
    assert udp_rx.hdr.dst_port = x"C350"            report "T2: udp_rx.hdr.dst_port not set correctly";

    assert udp_rx_start = '1'                       report "T2: udp_rx_start not set";
    assert udp_rx.data.data_in_valid = '1'          report "T2: udp_rx.data.data_in_valid not set";

    assert ip_rx_hdr.is_valid = '1'                 report "T2: ip_rx_hdr.is_valid not set";
    assert ip_rx_hdr.protocol = x"11"               report "T2: ip_rx_hdr.protocol not set correctly";
    assert ip_rx_hdr.src_ip_addr = x"c0a80001"      report "T2: ip_rx.hdr.src_ip_addr not set correctly";
    assert ip_rx_hdr.num_frame_errors = x"00"       report "T2: ip_rx.hdr.num_frame_errors not set correctly";
    assert ip_rx_hdr.last_error_code = x"0"         report "T2: ip_rx.hdr.last_error_code not set correctly";

    -- put the rest of the user data
    mac_rx_tdata <= x"00"; wait for 3*clk125_period; -- fill up the what to do word
    assert udp_rx.data.data_in_last = '1'           report "T2: udp_rx.data.data_in_last not set";
    wait for clk125_period;
    mac_rx_tlast <= '1'; wait for clk125_period;

    mac_rx_tdata <= x"00";
    mac_rx_tlast <= '0';
    mac_rx_tvalid <= '0';
    wait for clk125_period;

    assert udp_rx.data.data_in_valid = '0'          report "T2: udp_rx.data.data_in_valid not cleared";
    assert udp_rx.data.data_in_last = '0'           report "T2: udp_rx.data.data_in_last not cleared";
    assert udp_rx_start = '0'                       report "T2: udp_rx_start not cleared";
    assert ip_rx_hdr.num_frame_errors = x"00"       report "T2: ip_rx_hdr.num_frame_errors non zero at end of test";
    assert ip_rx_hdr.last_error_code = x"0"         report "T2: ip_rx_hdr.last_error_code indicates error at end of test";
    assert ip_pkt_count = x"01"                     report "T2: ip pkt cnt incorrect";

    wait for clk125_period*20;


    report "T2: test complete!";

    -- finish
    wait for 100 ms;
    assert false report "  Simulation finished." severity failure;
  end process;

end behavioral;
