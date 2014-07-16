--
--    -----------------------------------------------------------------------
--    | ML605 BASIC FIRMWARE                                                |
--    |             --------------------------------------------------------|
--    |             |FIFO BLOCK WRAPPER                                     |
--    |             |              -----------------------------------------|
--    |             |              | BLOCK LEVEL WRAPPER                    |
--    |             |              |                                        |
--    |             |              |    /-------------------\               |
--    |             |              |    |   V6 EMAC CORE    |               |
--    | /--------\  |              |    |                   |               |
--    | |        |  |              |    |                   |               |
--    | |  REG   |  |              |    |                   |               |
--    | | VALUES |  |              |    |                   |               |
--    | |        |  |  /--------\  |    |                   |               |
--    | \--------/  |  |        |  |    |                   |  /-------\    |
--    |     ||      |  |        |--|--->| Tx            Tx  |--|       |--->|
--    |     ||      |  |        |  |    | AXI-S         PHY |  |       |    |
--    | /--------\  |  |        |  |    | I/F           I/F |  |       |    |
--    | |        |  |  |  AXI-S |  |    |                   |  | PHY   |    |
--    | |        |->|->|  FIFO  |  |    |                   |  | I/F   |    |
--    | |        |  |  |        |  |    |                   |  |       |    |
--    | |  UDP   |  |  |        |  |    | Rx            Rx  |  |       |    |
--    | | MODULE |  |  |        |  |    | AX)-S         PHY |  |       |    |
--    | |        |<-|<-|        |<-|----| I/F           I/F |<-|       |<---|
--    | |        |  |  |        |  |    |                   |  \-------/    |
--    | \--------/  |  \--------/  |    \-------------------/               |
--    |             |              |                                        |
--    |             |              -----------------------------------------|
--    |             --------------------------------------------------------|
--    -----------------------------------------------------------------------
--
--------------------------------------------------------------------------------

library unisim;
use unisim.vcomponents.all;

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

-- udp stuff
use work.axi.all;
use work.ipv4_types.all;
use work.arp_types.all;

--------------------------------------------------------------------------------
-- The entity declaration for the example_design level wrapper.
--------------------------------------------------------------------------------

entity ethernet_core_wrapper is
  port (
    -- asynchronous reset
    GLBL_RST          : in  std_logic;

    -- input clocks from generator
    REFCLK_BUFG       : in  std_logic;  -- 200 MHz main clock
    GTX_CLK_BUFG      : in  std_logic;  -- 125 MHz GTX clock
    DCM_LOCKED        : in  std_logic;

    PHY_RESETN        : out std_logic;

    -- GMII Interface
    -----------------

    GMII_TXD          : out std_logic_vector(7 downto 0);
    GMII_TX_EN        : out std_logic;
    GMII_TX_ER        : out std_logic;
    GMII_TX_CLK       : out std_logic;
    GMII_RXD          : in  std_logic_vector(7 downto 0);
    GMII_RX_DV        : in  std_logic;
    GMII_RX_ER        : in  std_logic;
    GMII_RX_CLK       : in  std_logic;
    GMII_COL          : in  std_logic;
    GMII_CRS          : in  std_logic;
    MII_TX_CLK        : in  std_logic;

    -- RX_CLK            : out std_logic;

    -- Serialised statistics vectors
    --------------------------------
  --  TX_STATISTICS_S   : out std_logic;
  --  RX_STATISTICS_S   : out std_logic;

    -- Serialised Pause interface controls
    --------------------------------------
    PAUSE_REQ_S       : in  std_logic;

    -- Main example design controls
    -------------------------------
    DISPLAY           : out std_logic_vector (7 downto 0);
    RX_PKG_CTR        : out std_logic_vector (31 downto 0); -- pipe the package counter to the LCD

    REGISTER_ACCESS         : out std_logic;            -- a request to access the register
    REGISTER_ADDR           : out std_logic_vector(15 downto 0);  -- register address byte
    REGISTER_WRITE_OR_READ  : out std_logic;            -- should we write: '1'  or read: '0'
    REGISTER_READ_DATA      : in  std_logic_vector(31 downto 0);  -- data word for reading
    REGISTER_READ_READY     : in  std_logic;            -- register data is ready
    REGISTER_WRITE_DATA     : out std_logic_vector(31 downto 0);  -- data word for writing
    REGISTER_DMA            : out std_logic;            -- we want to access the DMA FIFO
    REGISTER_DMA_WAIT       : out std_logic;            -- wait a second with reading DMA, we need to do something
    REGISTER_DMA_END        : in  std_logic;            -- the DMA reading has ended
    REGISTER_DMA_EMPTY      : in  std_logic;            -- the DMA is empty
    REGISTER_DMA_COUNT      : in  std_logic_vector(17 downto 0);  -- how many words are in the DMA buffer?
    REGISTER_CLK            : in  std_logic

  );
end ethernet_core_wrapper;

architecture Behavorial of ethernet_core_wrapper is

  ------------------------------------------------------------------------------
  -- Component Declaration for the Tri-Mode EMAC core FIFO Block wrapper
  ------------------------------------------------------------------------------

  component ethernet_core_fifo_block
  port(
    gtx_clk               : in  std_logic;
    -- Receiver Statistics Interface
    -----------------------------------------
    rx_mac_aclk           : out std_logic;
    rx_reset              : out std_logic;
    rx_statistics_vector  : out std_logic_vector(27 downto 0);
    rx_statistics_valid   : out std_logic;

    -- Receiver (AXI-S) Interface
    ------------------------------------------
    rx_fifo_clock         : in  std_logic;
    rx_fifo_resetn        : in  std_logic;
    rx_axis_fifo_tdata    : out std_logic_vector(7 downto 0);
    rx_axis_fifo_tvalid   : out std_logic;
    rx_axis_fifo_tready   : in  std_logic;
    rx_axis_fifo_tlast    : out std_logic;

    -- Transmitter Statistics Interface
    --------------------------------------------
    tx_mac_aclk           : out std_logic;
    tx_reset              : out std_logic;
    tx_ifg_delay          : in  std_logic_vector(7 downto 0);
    tx_statistics_vector  : out std_logic_vector(31 downto 0);
    tx_statistics_valid   : out std_logic;

    -- Transmitter (AXI-S) Interface
    ---------------------------------------------
    tx_fifo_clock         : in  std_logic;
    tx_fifo_resetn        : in  std_logic;
    tx_axis_fifo_tdata    : in  std_logic_vector(7 downto 0);
    tx_axis_fifo_tvalid   : in  std_logic;
    tx_axis_fifo_tready   : out std_logic;
    tx_axis_fifo_tlast    : in  std_logic;

    -- MAC Control Interface
    --------------------------
    pause_req             : in  std_logic;
    pause_val             : in  std_logic_vector(15 downto 0);

    -- Reference clock for IDELAYCTRL's
    refclk                : in  std_logic;

    -- GMII Interface
    -------------------
    gmii_txd              : out std_logic_vector(7 downto 0);
    gmii_tx_en            : out std_logic;
    gmii_tx_er            : out std_logic;
    gmii_tx_clk           : out std_logic;
    gmii_rxd              : in  std_logic_vector(7 downto 0);
    gmii_rx_dv            : in  std_logic;
    gmii_rx_er            : in  std_logic;
    gmii_rx_clk           : in  std_logic;
    gmii_col              : in  std_logic;
    gmii_crs              : in  std_logic;
    mii_tx_clk            : in  std_logic;


    -- asynchronous reset
    glbl_rstn             : in  std_logic;
    rx_axi_rstn           : in  std_logic;
    tx_axi_rstn           : in  std_logic

  );
  end component;


  ------------------------------------------------------------------------------
  -- Component Declaration for UDP complete no mac
  ------------------------------------------------------------------------------

  COMPONENT UDP_Complete_nomac
  generic (
    CLOCK_FREQ        : integer := 125000000;       -- freq of data_in_clk -- needed to timout cntr
    ARP_TIMEOUT       : integer := 60;              -- ARP response timeout (s)
    ARP_MAX_PKT_TMO   : integer := 5;               -- # wrong nwk pkts received before set error
    MAX_ARP_ENTRIES   : integer := 255              -- max entries in the ARP store
  );
  Port (
    -- UDP TX signals
    udp_tx_start      : in  std_logic;              -- indicates req to tx UDP
    udp_txi           : in  udp_tx_type;            -- UDP tx cxns
    udp_tx_result     : out std_logic_vector (1 downto 0);-- tx status (changes during transmission)
    udp_tx_data_out_ready : out std_logic;          -- indicates udp_tx is ready to take data
    -- UDP RX signals
    udp_rx_start      : out std_logic;              -- indicates receipt of udp header
    udp_rxo           : out udp_rx_type;
    -- IP RX signals
    ip_rx_hdr         : out ipv4_rx_header_type;
    -- system signals
    rx_clk            : in  STD_LOGIC;
    tx_clk            : in  STD_LOGIC;
    reset             : in  STD_LOGIC;
    our_ip_address    : in  STD_LOGIC_VECTOR (31 downto 0);
    our_mac_address   : in  std_logic_vector (47 downto 0);
    control           : in  udp_control_type;
    -- status signals
    arp_pkt_count     : out STD_LOGIC_VECTOR(7 downto 0);   -- count of arp pkts received
    ip_pkt_count      : out STD_LOGIC_VECTOR(7 downto 0);   -- number of IP pkts received for us
    -- MAC Transmitter
    mac_tx_tdata      : out std_logic_vector(7 downto 0);   -- data byte to tx
    mac_tx_tvalid     : out std_logic;             -- tdata is valid
    mac_tx_tready     : in  std_logic;             -- mac is ready to accept data
    mac_tx_tfirst     : out std_logic;             -- indicates first byte of frame
    mac_tx_tlast      : out std_logic;             -- indicates last byte of frame
    -- MAC Receiver
    mac_rx_tdata      : in  std_logic_vector(7 downto 0);   -- data byte received
    mac_rx_tvalid     : in  std_logic;             -- indicates tdata is valid
    mac_rx_tready     : out std_logic;             -- tells mac that we are ready to take data
    mac_rx_tlast      : in  std_logic              -- indicates last byte of the trame
  );
  END COMPONENT;


  ------------------------------------------------------------------------------
  -- Component declaration for the reset synchroniser
  ------------------------------------------------------------------------------
  component reset_sync
  port (
     reset_in   : in  std_logic;    -- Active high asynchronous reset
     enable     : in  std_logic;
     clk        : in  std_logic;    -- clock to be sync'ed to
     reset_out  : out std_logic     -- "Synchronised" reset signal
  );
  end component;

  ------------------------------------------------------------------------------
  -- Component declaration for the synchroniser
  ------------------------------------------------------------------------------
  component sync_block
  port (
     clk        : in  std_logic;
     data_in    : in  std_logic;
     data_out   : out std_logic
  );
  end component;

   ------------------------------------------------------------------------------
   -- Constants used in this top level wrapper.
   ------------------------------------------------------------------------------

  constant our_ip   : std_logic_vector (31 downto 0) := x"c0a8000a";    -- 192.168.0.10
  constant our_mac  : std_logic_vector (47 downto 0) := x"000a35022e4d";


  ------------------------------------------------------------------------------
  -- internal signals used in this top level wrapper.
  ------------------------------------------------------------------------------

  signal rx_mac_aclk              : std_logic;
  signal tx_mac_aclk              : std_logic;


  signal phy_resetn_int           : std_logic;

  -- resets (and reset generation)
  -- signal local_chk_reset         : std_logic;
  signal chk_reset_int            : std_logic;
  -- signal chk_pre_resetn          : std_logic := '0';
  -- signal chk_resetn              : std_logic := '0';
  signal local_gtx_reset          : std_logic;
  signal gtx_clk_reset_int        : std_logic;
  signal gtx_pre_resetn           : std_logic := '0';
  signal gtx_resetn               : std_logic := '0';
  signal rx_reset                 : std_logic;
  signal tx_reset                 : std_logic;

  --signal dcm_locked               : std_logic;
  signal glbl_rst_int             : std_logic;
  signal phy_reset_count                   : unsigned(5 downto 0);
  signal glbl_rst_intn            : std_logic;

  -- USER side RX AXI-S interface
  signal rx_fifo_clock            : std_logic;
  signal rx_fifo_resetn           : std_logic;
  signal rx_axis_fifo_tdata       : std_logic_vector(7 downto 0);
  signal rx_axis_fifo_tvalid      : std_logic;
  signal rx_axis_fifo_tlast       : std_logic;
  signal rx_axis_fifo_tready      : std_logic;

  -- USER side TX AXI-S interface
  signal tx_fifo_clock            : std_logic;
  signal tx_fifo_resetn           : std_logic;
  signal tx_axis_fifo_tdata       : std_logic_vector(7 downto 0);
  signal tx_axis_fifo_tvalid      : std_logic;
  signal tx_axis_fifo_tlast       : std_logic;
  signal tx_axis_fifo_tready      : std_logic;

  -- RX Statistics serialisation signals
  signal rx_statistics_valid      : std_logic;
  signal rx_statistics_valid_reg  : std_logic;
  signal rx_statistics_vector     : std_logic_vector(27 downto 0);
  signal rx_stats                 : std_logic_vector(27 downto 0);
  signal rx_stats_toggle          : std_logic := '0';
  signal rx_stats_toggle_sync     : std_logic;
  signal rx_stats_toggle_sync_reg : std_logic := '0';
  signal rx_stats_shift           : std_logic_vector(29 downto 0);

  -- TX Statistics serialisation signals
  signal tx_statistics_valid      : std_logic;
  signal tx_statistics_valid_reg  : std_logic;
  signal tx_statistics_vector     : std_logic_vector(31 downto 0);
  signal tx_stats                 : std_logic_vector(31 downto 0);
  signal tx_stats_toggle          : std_logic := '0';
  signal tx_stats_toggle_sync     : std_logic;
  signal tx_stats_toggle_sync_reg : std_logic := '0';
  signal tx_stats_shift           : std_logic_vector(33 downto 0);

  -- Pause interface DESerialisation
  signal pause_shift              : std_logic_vector(17 downto 0);
  signal pause_req                : std_logic;
  signal pause_val                : std_logic_vector(15 downto 0);



  -- signal tie offs
  constant tx_ifg_delay           : std_logic_vector(7 downto 0) := (others => '0');    -- not used in this example
  signal int_frame_error          : std_logic;

  attribute keep : string;
  attribute keep of gtx_clk_bufg             : signal is "true";
  attribute keep of refclk_bufg              : signal is "true";
  attribute keep of rx_statistics_valid      : signal is "true";
  attribute keep of rx_statistics_vector     : signal is "true";
  attribute keep of tx_statistics_valid      : signal is "true";
  attribute keep of tx_statistics_vector     : signal is "true";



  ------------------------------------------------------------------------------
  -- stuff for the UDP module
  ------------------------------------------------------------------------------

  type state_type is (IDLE, WAIT_RX_DONE, WAIT_DATA_OUT_RDY, DATA_OUT, FINISH_SEND, PREPARE_NEXT_PKG, PREPARE_NEXT_PKG_2);
  type count_mode_type is (RST, INCR, HOLD);
  type set_clr_type is (SET, CLR, HOLD);
  --type get_dma_data_state_type is (IDLE, TRANSFER, FIN);

  -- system signals
  signal udp_tx_int           : udp_tx_type;
  signal udp_tx_result_int    : std_logic_vector (1 downto 0);
  signal udp_tx_data_out_ready_int  : std_logic;
  signal udp_rx_int           : udp_rx_type;
  signal udp_tx_start_int     : std_logic;
  signal udp_rx_start_int     : std_logic;
  signal arp_pkt_count_int    : STD_LOGIC_VECTOR(7 downto 0);
  signal ip_pkt_count_int     : STD_LOGIC_VECTOR(7 downto 0);
  signal ip_rx_hdr_int        : ipv4_rx_header_type;

  -- what_to_do is the first byte of the UDP data payload and is
  --  an identifyer, that tells us what should be done.
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
  constant do_read_available_data : what_to_do_type := x"24";  -- read available data
  signal what_to_do           : what_to_do_type;

  -- state signals
  signal state                : state_type;
  signal rx_count             : unsigned (7 downto 0) := x"00";
 
  signal nextpkg_count						: unsigned (7 downto 0) := x"00";--wait few clock cycles for the next package to prevent tx errors

  signal tx_count             : unsigned (15 downto 0) := x"0000";
  signal waiting_for_write_count    : unsigned (11 downto 0) := x"000";
  signal timeout_for_waiting_count  : unsigned (7 downto 0) := x"00";
  signal rx_pkg_ctr_int       : std_logic_vector(31 downto 0);  -- how many UDP packages were received?
  signal tx_count_target      : integer range 0 to 1500 := 0;  -- how many bytes should we send back?
  constant timeout_for_waiting  : integer := 20;  -- how many cycles should we wait until sending back a timeout?
  constant waiting_for_write    : integer := 100;  -- how many cycles should we wait for the writing process?
  signal tx_hdr               : udp_tx_header_type;
  signal tx_start_reg         : std_logic;
 -- signal tx_started_reg       : std_logic;
  signal tx_fin_reg           : std_logic;
  constant dma_reply			   	: std_logic_vector(7 downto 0) := x"22";
  constant dma_last_reply				: std_logic_vector(7 downto 0) := x"23";
  signal dma_what_to_reply      	: std_logic_vector(7 downto 0) := x"23";
  signal dma_reply_additional_information	   : std_logic_vector(23 downto 0) := x"000000"; -- counts the number of packages 
	

  -- control signals
  signal next_state           : state_type;
  signal set_state            : std_logic;
  signal set_rx_count         : count_mode_type;
  signal set_nextpkg_count					: count_mode_type;
  signal set_dma_reply_additional_information		: count_mode_type;
  signal set_tx_count         : count_mode_type;
  signal set_timeout_for_waiting_count  : count_mode_type;
  signal set_waiting_for_write_count    : count_mode_type;
  signal set_pkg_count        : count_mode_type;
  signal set_hdr              : std_logic;
  signal set_tx_start         : set_clr_type;
  signal set_last             : std_logic;
--  signal set_tx_started       : set_clr_type;
  signal set_tx_fin           : set_clr_type;
  signal control_int          : udp_control_type;
  signal set_timeout_for_waiting_exeeded  : std_logic;

  -- control what we should do
  signal set_what_to_do             : std_logic;
  signal set_what_to_do_nextpkg     : std_logic;
  signal reset_what_to_do           : std_logic;
  signal set_register_access        : std_logic;
--  signal unset_register_access      : std_logic;
  signal reset_register_access      : std_logic;
  --signal set_dma_access             : std_logic;
  signal reset_dma_access           : std_logic;

  signal set_register_addr1         : std_logic;
  signal set_register_addr2         : std_logic;
  signal set_register_addr_zero     : std_logic;
  signal set_register_write_data1   : std_logic;
  signal set_register_write_data2   : std_logic;
  signal set_register_write_data3   : std_logic;
  signal set_register_write_data4   : std_logic;
  signal set_register_read_dma_cnt1 : std_logic;
  signal set_register_read_dma_cnt2 : std_logic;
  signal set_register_read_dma_cnt3 : std_logic;
  signal set_register_read_dma_cnt4 : std_logic;
  signal set_register_dma_count     : std_logic;
  signal set_fifo_dma_read_next     : std_logic;
  --signal set_register_read_data     : std_logic;

  -- register handling
  signal register_access_int         : std_logic;
  signal register_access_clk_sync    : std_logic;
  signal register_access_delayed     : std_logic;
  -- signal register_access_delayed_2   : std_logic;
  signal register_read_ready_delayed : std_logic;
  -- signal register_dma_empty_delayed  : std_logic;
  signal register_write_or_read_int  : std_logic; -- write: '1'  read: '0'
  signal register_addr_int           : std_logic_vector(15 downto 0);
  signal register_write_data_int     : std_logic_vector(31 downto 0);
  signal register_read_data_int      : std_logic_vector(31 downto 0);
  --signal wait_with_register_access   : std_logic;

  -- dma block fifo
  signal register_dma_count_int   : std_logic_vector(31 downto 0);
  signal register_dma_int         : std_logic;
--  signal register_dma_is_empty    : std_logic;
  -- signal register_dma_access_init : std_logic;
  signal fifo_dma_reset           : std_logic;
  signal fifo_dma_din             : std_logic_vector(31 downto 0);
  signal fifo_dma_dout            : std_logic_vector(31 downto 0);
  signal fifo_dma_write_en        : std_logic;
  signal fifo_dma_read_en         : std_logic;
  signal fifo_dma_full            : std_logic;
  signal fifo_dma_empty           : std_logic;
  signal fifo_dma_transfer_end    : std_logic;
  signal finish_get_dma_data      : std_logic;
  signal fifo_data_count			 : std_logic_vector(9 downto 0);

  -- test output for the user LEDs
  signal test_display_output      : std_logic;
  signal test_display_int         : std_logic;

   -- dma read signals
  signal how_many_left 				 : std_logic_vector(31 downto 0) := x"00000000";
  signal remember_addr				 :	std_logic_vector(15 downto 0) := x"0000";
  signal set_next_register_read_dma_cnt   : std_logic;
  signal keep_register_addr       : std_logic;
  signal more_than_one_pkg   		 : std_logic;


  ------------------------------------------------------------------------------
  -- Begin architecture
  ------------------------------------------------------------------------------

begin


  -----------------
  -- global reset

  glbl_reset_gen : reset_sync
  port map (
    clk         => GTX_CLK_BUFG,
    enable      => DCM_LOCKED,
    reset_in    => GLBL_RST,
    reset_out   => glbl_rst_int
  );

  glbl_rst_intn <= not glbl_rst_int;

  -- generate the user side clocks for the axi fifos
  tx_fifo_clock <= GTX_CLK_BUFG;
  rx_fifo_clock <= GTX_CLK_BUFG;

  ------------------------------------------------------------------------------
  -- Generate resets required for the fifo side signals plus axi_lite logic
  ------------------------------------------------------------------------------
  -- in each case the async reset is first captured and then synchronised


  --local_chk_reset <= glbl_rst;-- or reset_error;

  -----------------
  -- data check reset
  chk_reset_gen : reset_sync
  port map (
    clk         => GTX_CLK_BUFG,
    enable      => DCM_LOCKED,
    reset_in    => GLBL_RST, --local_chk_reset,
    reset_out   => chk_reset_int
  );

   -- Create fully synchronous reset in the gtx clock domain.
--   gen_chk_reset : process (gtx_clk_bufg)
--   begin
--     if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
--       if chk_reset_int = '1' then
--         chk_pre_resetn   <= '0';
--         chk_resetn       <= '0';
--       else
--         chk_pre_resetn   <= '1';
--         chk_resetn       <= chk_pre_resetn;
--       end if;
--     end if;
--   end process gen_chk_reset;

  local_gtx_reset <= GLBL_RST or rx_reset or tx_reset;

  -----------------
  -- gtx_clk reset
  gtx_reset_gen : reset_sync
  port map (
    clk              => gtx_clk_bufg,
    enable           => dcm_locked,
    reset_in         => local_gtx_reset,
    reset_out        => gtx_clk_reset_int
  );

  -- Create fully synchronous reset in the s_axi clock domain.
  gen_gtx_reset : process (gtx_clk_bufg)
  begin
    if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
      if gtx_clk_reset_int = '1' then
        gtx_pre_resetn   <= '0';
        gtx_resetn       <= '0';
      else
        gtx_pre_resetn   <= '1';
        gtx_resetn       <= gtx_pre_resetn;
      end if;
    end if;
  end process gen_gtx_reset;


  -----------------
  -- PHY reset
  -- the phy reset output (active low) needs to be held for at least 10x25MHZ cycles
  -- this is derived using the 125MHz available and a 6 bit counter
  gen_phy_reset : process (gtx_clk_bufg)
  begin
    if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
      if glbl_rst_intn = '0' then
        phy_resetn_int       <= '0';
        phy_reset_count      <= (others => '0');
      else
        if phy_reset_count /= "111111" then
          phy_reset_count <= phy_reset_count + "000001";
        else
          phy_resetn_int   <= '1';
        end if;
      end if;
    end if;
  end process gen_phy_reset;

  phy_resetn <= phy_resetn_int;

  -- generate the user side resets for the axi fifos
  tx_fifo_resetn <= gtx_resetn;
  rx_fifo_resetn <= gtx_resetn;

  ------------------------------------------------------------------------------
  -- Serialize the stats vectors
  -- This is a single bit approach, retimed onto gtx_clk
  -- this code is only present to prevent code being stripped..
  ------------------------------------------------------------------------------

  -- RX STATS

  -- first capture the stats on the appropriate clock
  capture_rx_stats : process (rx_mac_aclk)
  begin
    if rx_mac_aclk'event and rx_mac_aclk = '1' then
      rx_statistics_valid_reg <= rx_statistics_valid;
      if rx_statistics_valid_reg = '0' and rx_statistics_valid = '1' then
        rx_stats        <= rx_statistics_vector;
        rx_stats_toggle <= not rx_stats_toggle;
      end if;
    end if;
  end process capture_rx_stats;

  rx_stats_sync : sync_block
  port map (
    clk              => gtx_clk_bufg,
    data_in          => rx_stats_toggle,
    data_out         => rx_stats_toggle_sync
  );

  reg_rx_toggle : process (gtx_clk_bufg)
  begin
    if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
      rx_stats_toggle_sync_reg <= rx_stats_toggle_sync;
    end if;
  end process reg_rx_toggle;

  -- when an update is rxd load shifter (plus start/stop bit)
  -- shifter always runs (no power concerns as this is an example design)
  gen_shift_rx : process (gtx_clk_bufg)
  begin
    if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
      if (rx_stats_toggle_sync_reg xor rx_stats_toggle_sync) = '1' then
        rx_stats_shift <= '1' & rx_stats &  '1';
      else
        rx_stats_shift <= rx_stats_shift(28 downto 0) & '0';
      end if;
    end if;
  end process gen_shift_rx;

--  rx_statistics_s <= rx_stats_shift(29);

  -- TX STATS

  -- first capture the stats on the appropriate clock
  capture_tx_stats : process (tx_mac_aclk)
  begin
    if tx_mac_aclk'event and tx_mac_aclk = '1' then
      tx_statistics_valid_reg <= tx_statistics_valid;
      if tx_statistics_valid_reg = '0' and tx_statistics_valid = '1' then
        tx_stats        <= tx_statistics_vector;
        tx_stats_toggle <= not tx_stats_toggle;
      end if;
    end if;
  end process capture_tx_stats;

  tx_stats_sync : sync_block
  port map (
    clk              => gtx_clk_bufg,
    data_in          => tx_stats_toggle,
    data_out         => tx_stats_toggle_sync
  );

  reg_tx_toggle : process (gtx_clk_bufg)
  begin
    if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
      tx_stats_toggle_sync_reg <= tx_stats_toggle_sync;
    end if;
  end process reg_tx_toggle;

  -- when an update is txd load shifter (plus start bit)
  -- shifter always runs (no power concerns as this is an example design)
  gen_shift_tx : process (gtx_clk_bufg)
  begin
    if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
      if (tx_stats_toggle_sync_reg xor tx_stats_toggle_sync) = '1' then
        tx_stats_shift <= '1' & tx_stats & '1';
      else
        tx_stats_shift <= tx_stats_shift(32 downto 0) & '0';
      end if;
    end if;
  end process gen_shift_tx;

 -- tx_statistics_s <= tx_stats_shift(33);

  ------------------------------------------------------------------------------
  -- DESerialize the Pause interface
  -- This is a single bit approach timed on gtx_clk
  -- this code is only present to prevent code being stripped..
  ------------------------------------------------------------------------------
  -- the serialized pause info has a start bit followed by the quanta and a stop bit
  -- capture the quanta when the start bit hits the msb and the stop bit is in the lsb
  gen_shift_pause : process (gtx_clk_bufg)
  begin
    if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
      pause_shift <= pause_shift(16 downto 0) & pause_req_s;
    end if;
  end process gen_shift_pause;

  grab_pause : process (gtx_clk_bufg)
  begin
    if gtx_clk_bufg'event and gtx_clk_bufg = '1' then
      if (pause_shift(17) = '1' and pause_shift(0) = '1') then
        pause_req <= '1';
        pause_val <= pause_shift(16 downto 1);
      else
        pause_req <= '0';
        pause_val <= (others => '0');
      end if;
    end if;
  end process grab_pause;


  ------------------------------------------------------------------------------
  -- Instantiate the V6 Hard MAC core FIFO Block wrapper
  ------------------------------------------------------------------------------
  v6emac_fifo_block : ethernet_core_fifo_block
  port map (
    gtx_clk               => gtx_clk_bufg,
    -- Reference clock for IDELAYCTRL's
    refclk                => refclk_bufg,

    -- Receiver Statistics Interface
    -----------------------------------------
    rx_mac_aclk           => rx_mac_aclk,
    rx_reset              => rx_reset,
    rx_statistics_vector  => rx_statistics_vector,
    rx_statistics_valid   => rx_statistics_valid,

    -- Receiver => AXI-S Interface
    ------------------------------------------
    rx_fifo_clock         => rx_fifo_clock,
    rx_fifo_resetn        => rx_fifo_resetn,
    rx_axis_fifo_tdata    => rx_axis_fifo_tdata,
    rx_axis_fifo_tvalid   => rx_axis_fifo_tvalid,
    rx_axis_fifo_tready   => rx_axis_fifo_tready,
    rx_axis_fifo_tlast    => rx_axis_fifo_tlast,

    -- Transmitter Statistics Interface
    --------------------------------------------
    tx_mac_aclk           => tx_mac_aclk,
    tx_reset              => tx_reset,
    tx_ifg_delay          => tx_ifg_delay,
    tx_statistics_vector  => tx_statistics_vector,
    tx_statistics_valid   => tx_statistics_valid,

    -- Transmitter => AXI-S Interface
    ---------------------------------------------
    tx_fifo_clock         => tx_fifo_clock,
    tx_fifo_resetn        => tx_fifo_resetn,
    tx_axis_fifo_tdata    => tx_axis_fifo_tdata,
    tx_axis_fifo_tvalid   => tx_axis_fifo_tvalid,
    tx_axis_fifo_tready   => tx_axis_fifo_tready,
    tx_axis_fifo_tlast    => tx_axis_fifo_tlast,

    -- MAC Control Interface
    --------------------------
    pause_req             => pause_req,
    pause_val             => pause_val,

    -- GMII Interface
    -------------------
    gmii_txd              => gmii_txd,
    gmii_tx_en            => gmii_tx_en,
    gmii_tx_er            => gmii_tx_er,
    gmii_tx_clk           => gmii_tx_clk,
    gmii_rxd              => gmii_rxd,
    gmii_rx_dv            => gmii_rx_dv,
    gmii_rx_er            => gmii_rx_er,
    gmii_rx_clk           => gmii_rx_clk,
    gmii_col              => gmii_col,
    gmii_crs              => gmii_crs,
    mii_tx_clk            => mii_tx_clk,


    -- asynchronous reset
    glbl_rstn             => glbl_rst_intn,
    rx_axi_rstn           => '1',
    tx_axi_rstn           => '1'

  );



-- ========================================================================================
-- ========================================================================================
-- ========================================================================================
-- ========================================================================================
-- ========================================================================================




  ------------------------------------------------------------------------------
  --  Instantiate the UDP and ARP modules
  ------------------------------------------------------------------------------



  udp_block: UDP_Complete_nomac
  generic map (
    ARP_TIMEOUT       => 10     -- timeout in seconds
  )
  PORT MAP (
    -- UDP TX signals
    udp_tx_start      => udp_tx_start_int,
    udp_txi           => udp_tx_int,
    udp_tx_result     => udp_tx_result_int,
    udp_tx_data_out_ready => udp_tx_data_out_ready_int,
    -- UDP RX signals
    udp_rx_start      => udp_rx_start_int,
    udp_rxo           => udp_rx_int,
    -- IP RX signals
    ip_rx_hdr         => ip_rx_hdr_int,
    -- system signals
    rx_clk            => gtx_clk_bufg,
    tx_clk            => gtx_clk_bufg,
    reset             => glbl_rst_int,
    our_ip_address    => our_ip,
    our_mac_address   => our_mac,
    -- status signals
    arp_pkt_count     => arp_pkt_count_int,
    ip_pkt_count      => ip_pkt_count_int,
    control           => control_int,
    -- MAC Transmitter
    mac_tx_tready     => tx_axis_fifo_tready,
    mac_tx_tvalid     => tx_axis_fifo_tvalid,
    mac_tx_tfirst     => open,
    mac_tx_tlast      => tx_axis_fifo_tlast,
    mac_tx_tdata      => tx_axis_fifo_tdata,
    -- MAC Receiver
    mac_rx_tdata      => rx_axis_fifo_tdata,
    mac_rx_tvalid     => rx_axis_fifo_tvalid,
    mac_rx_tready     => rx_axis_fifo_tready,
    mac_rx_tlast      => rx_axis_fifo_tlast
  );



  Test_Display : entity work.led_eine_sec
  port map (
    CLK       => gtx_clk_bufg,
    TRIGGER   => test_display_int,
    LED_TEST  => open,
    LED_OUT   => test_display_output
  );


  process ( ip_pkt_count_int, arp_pkt_count_int, test_display_output )
  begin
    control_int.ip_controls.arp_controls.clear_cache <= '0';

    -- set display leds to show IP pkt rx count on 7..4 and arp rx count on 3..0
    --display (3 downto 0) <= arp_pkt_count_int (3 downto 0);
    DISPLAY (7 downto 4) <= ip_pkt_count_int (3 downto 0);

    DISPLAY (3 downto 1) <= "000";
    --DISPLAY (3 downto 1) <= std_logic_vector(to_unsigned(tx_count_target+1,3));
    DISPLAY(0) <= test_display_output;

  end process;




  -- AUTO TX process - on receipt of any UDP pkt, send a response. data sent is modified if a broadcast was received.

  -- TX response process - COMB
  tx_proc_combinatorial: process( tx_mac_aclk,
    -- inputs
    udp_rx_start_int, udp_rx_int, udp_tx_data_out_ready_int, udp_tx_result_int, ip_rx_hdr_int,
    udp_tx_int.data.data_out_valid, what_to_do, tx_count_target, timeout_for_waiting_count,
    register_access_int, register_read_data_int, REGISTER_READ_READY,
    fifo_dma_dout, fifo_dma_empty,
    -- state
    state, rx_count, nextpkg_count, dma_reply_additional_information, tx_count, tx_hdr, tx_start_reg, tx_fin_reg,
    waiting_for_write_count, rx_pkg_ctr_int,
    -- controls
    next_state, set_state, set_rx_count, set_nextpkg_count, set_dma_reply_additional_information, set_tx_count, set_hdr, set_tx_start,
    set_last, set_tx_fin )
	 
    variable continue_to_send_data : std_logic;
  begin

    -- set output_followers
    udp_tx_int.hdr <= tx_hdr;
    udp_tx_int.data.data_out_last <= set_last;
    udp_tx_start_int <= tx_start_reg;

    -- set control signal defaults
    next_state <= IDLE;
    set_state <= '0';
    set_rx_count <= HOLD;
	 set_nextpkg_count <= HOLD;
	 set_dma_reply_additional_information <= HOLD;
    set_tx_count <= HOLD;
    set_waiting_for_write_count <= HOLD;
    set_timeout_for_waiting_count <= HOLD;
    set_hdr <= '0';
    set_tx_start <= HOLD;
    set_last <= '0';
    --set_tx_started <= HOLD;
    set_tx_fin <= HOLD;
    set_timeout_for_waiting_exeeded <= '0';
    set_pkg_count <= HOLD;

    set_what_to_do <= '0';
	 set_what_to_do_nextpkg <= '0';
    set_register_access <= '0';
   -- unset_register_access <= '0';
    set_register_addr1 <= '0';
    set_register_addr2 <= '0';
    set_register_addr_zero <= '0';
    set_register_write_data1 <= '0';
    set_register_write_data2 <= '0';
    set_register_write_data3 <= '0';
    set_register_write_data4 <= '0';
    set_register_dma_count <= '0';
    set_register_read_dma_cnt1 <= '0';
    set_register_read_dma_cnt2 <= '0';
	 set_register_read_dma_cnt3 <= '0';
	 set_register_read_dma_cnt4 <= '0';
    -- set_register_read_data <= '0';
    set_fifo_dma_read_next <= '0';
    reset_register_access <= '0';
    reset_dma_access <= '0';
    reset_what_to_do <= '0';

    test_display_int <= '0';

	 set_next_register_read_dma_cnt <= '0';
	 keep_register_addr <= '0';


    -- FSM
    case state is

      when IDLE =>
        udp_tx_int.data.data_out_valid <= '0';
        if udp_rx_start_int = '1' then
          if ( udp_rx_int.data.data_in_valid = '1' ) then
            set_what_to_do <= '1';
            set_pkg_count <= INCR;
          end if;

          set_tx_fin <= CLR;
			 set_nextpkg_count <= RST;
			 set_dma_reply_additional_information <= RST;
          set_rx_count <= RST;
          set_tx_count <= RST;
          set_waiting_for_write_count <= RST;
          set_timeout_for_waiting_count <= RST;
          if udp_rx_int.data.data_in_last = '1' then
            next_state <= WAIT_DATA_OUT_RDY;
            set_state <= '1';
          else
            next_state <= WAIT_RX_DONE;
            set_state <= '1';
          end if;
        end if;


      when WAIT_RX_DONE =>
        -- wait until RX pkt fully received
        if ( udp_rx_int.data.data_in_valid = '1' ) then
          -- more data is coming in ...
          -- bytes 2-4 from the first 32-bit-integer are always 0 (x01, x02 and x03)
          set_rx_count <= INCR;

          if ( what_to_do = do_write_register ) then
            case rx_count is
              -- first two bytes of the address are always 0 (x04 and x05)
              when x"05"  => set_register_addr1 <= '1';
              when x"06"  => set_register_addr2 <= '1';
              when x"07"  => set_register_write_data1 <= '1';
              when x"08"  => set_register_write_data2 <= '1';
              when x"09"  => set_register_write_data3 <= '1';
              when x"0a"  => set_register_write_data4 <= '1'; set_register_access <= '1';

              when others =>  -- do nothing
            end case;

          elsif ( what_to_do = do_read_register ) then
            case rx_count is
              -- first two bytes of the address are always 0 (x04 and x05)
              when x"05"  => set_register_addr1 <= '1';
              when x"06"  => set_register_addr2 <= '1'; set_register_access <= '1';
              when others =>  -- do nothing
            end case;

          elsif ( what_to_do = do_read_dma ) then
            case rx_count is
              -- first two bytes of the address are always 0 (x04 and x05)
              when x"05"  => set_register_addr1 <= '1';
              when x"06"  => set_register_addr2 <= '1';
							-- first two bytes of the read count are always 0 (x07 and x08) //32 bit now
							
				  when x"07"	=> set_register_read_dma_cnt1 <= '1';
				  when x"08"	=> set_register_read_dma_cnt2 <= '1';
				  when x"09"	=> set_register_read_dma_cnt3 <= '1';
				  when x"0a"	=> set_register_read_dma_cnt4 <= '1';-- set_register_access <= '1';
				  when others =>  -- do nothing

            end case;
          end if;

        end if;

        if udp_rx_int.data.data_in_last = '1' then
          next_state <= WAIT_DATA_OUT_RDY;
          set_state <= '1';
        end if;


      when WAIT_DATA_OUT_RDY =>
        continue_to_send_data := '0';

        if ( what_to_do = do_read_register ) then
          if ( register_access_int = '0' ) then
            -- OK, there was a request for reading a register, but no address
            -- came with the request. Assume, that the address 0x00 is requested.
            set_register_access <= '1';
            set_register_addr_zero <= '1';
          -- elsif ( REGISTER_READ_READY = '1' ) then
          --  --set_register_read_data <= '1';
          --  continue_to_send_data := '1';
          end if;
          continue_to_send_data := '1';

        elsif ( what_to_do = do_write_register ) then
          -- if ( waiting_for_write_count = waiting_for_write-1 ) then
          --  set_timeout_for_waiting_exeeded <= '1';
          --  continue_to_send_data := '1';
          -- elsif ( REGISTER_READ_READY = '1' ) then
            continue_to_send_data := '1';
            -- unset_register_access <= '1';
          -- else
          --  set_waiting_for_write_count <= INCR;
          -- end if;

        elsif ( what_to_do = do_read_dma ) then
          if ( register_access_int = '0' ) then
            set_register_dma_count <= '1';
            set_register_access <= '1';
          else
            continue_to_send_data := '1';
          end if;
		  elsif ( what_to_do = do_read_available_data) then
	       continue_to_send_data := '1';

        elsif ( what_to_do = x"aa" ) then
          next_state <= IDLE;
          set_state <= '1';

        else
          continue_to_send_data := '1';
        end if;

        -- include a timeout, otherwise the logic might get stuck with waiting
        if ( continue_to_send_data = '0' and timeout_for_waiting_count = timeout_for_waiting-1 ) then
          set_timeout_for_waiting_exeeded <= '1';
          continue_to_send_data := '1';
        elsif ( what_to_do /= do_write_register ) then
          set_timeout_for_waiting_count <= INCR;
        end if;

        if ( continue_to_send_data = '1' ) then
          set_hdr <= '1';
        --  set_tx_started <= SET;
          set_tx_start <= SET;
          next_state <= DATA_OUT;
          set_state <= '1';
        end if;


      when DATA_OUT =>
        if udp_tx_result_int = UDPTX_RESULT_ERR then
          -- have an error from the IP TX layer, clear down the TX
          set_tx_start <= CLR;
          set_tx_fin <= SET;
       --  set_tx_started <= CLR;
          next_state <= IDLE;
          set_state <= '1';
        else
          if udp_tx_result_int = UDPTX_RESULT_SENDING then
            set_tx_start <= CLR;    -- reset out start req as soon as we know we are sending
          end if;

          -- what data should we send?
          if ip_rx_hdr_int.is_broadcast = '1' then
            udp_tx_int.data.data_out <= std_logic_vector(rx_count) or x"50";
          else
            case what_to_do is
              when do_ping =>
                if ( tx_count = 0 ) then
                  udp_tx_int.data.data_out <= do_pong;
                else
                  udp_tx_int.data.data_out <= (others => '0');
                end if;

              when do_read_register =>
                case tx_count(7 downto 0) is
                  when x"00"  => udp_tx_int.data.data_out <= do_read_register;
                  when x"01"  => udp_tx_int.data.data_out <= (others => '0');
                  when x"02"  => udp_tx_int.data.data_out <= (others => '0');
                  when x"03"  => udp_tx_int.data.data_out <= (others => '0');
                  when x"04"  => udp_tx_int.data.data_out <= register_read_data_int(31 downto 24);
                  when x"05"  => udp_tx_int.data.data_out <= register_read_data_int(23 downto 16);
                  when x"06"  => udp_tx_int.data.data_out <= register_read_data_int(15 downto 8);
                  when x"07"  => udp_tx_int.data.data_out <= register_read_data_int(7 downto 0);
                  when others => udp_tx_int.data.data_out <= (others => '0');
                end case;

              when do_write_register =>
                if ( tx_count = 0 ) then
                  udp_tx_int.data.data_out <= do_confirm;
                else
                  udp_tx_int.data.data_out <= (others => '0');
                end if;

              when do_pkg_count_read =>
                case tx_count(7 downto 0) is
                  when x"00"  => udp_tx_int.data.data_out <= do_pkg_count_read;
                  when x"01"  => udp_tx_int.data.data_out <= (others => '0');
                  when x"02"  => udp_tx_int.data.data_out <= (others => '0');
                  when x"03"  => udp_tx_int.data.data_out <= (others => '0');
                  when x"04"  => udp_tx_int.data.data_out <= rx_pkg_ctr_int(31 downto 24);
                  when x"05"  => udp_tx_int.data.data_out <= rx_pkg_ctr_int(23 downto 16);
                  when x"06"  => udp_tx_int.data.data_out <= rx_pkg_ctr_int(15 downto 8);
                  when x"07"  => udp_tx_int.data.data_out <= rx_pkg_ctr_int(7 downto 0);
                  when others => udp_tx_int.data.data_out <= (others => '0');
                end case;

					when do_read_available_data =>
						case tx_count (1 downto 0) is
							when "00"  => udp_tx_int.data.data_out <= do_read_available_data ;
							when "01"	=> udp_tx_int.data.data_out (7 downto 1) <= (others =>'0');
													   udp_tx_int.data.data_out (1 downto 0) <= REGISTER_DMA_COUNT (17 downto 16);
							when "10"	=> udp_tx_int.data.data_out <= REGISTER_DMA_COUNT (15 downto 8);
							when "11"	=> udp_tx_int.data.data_out <= REGISTER_DMA_COUNT (7 downto 0);
							when others => udp_tx_int.data.data_out <= (others => '0');
						end case;



              when do_pkg_count_reset =>
                set_pkg_count <= RST;
                if ( tx_count = 0 ) then
                  udp_tx_int.data.data_out <= do_confirm;
                else
                  udp_tx_int.data.data_out <= (others => '0');
                end if;

              when do_read_dma =>
				  		if (more_than_one_pkg = '1') then
						dma_what_to_reply <= dma_reply;
        --        if ( register_dma_is_empty = '1' ) then
        --          -- no data available
        --          udp_tx_int.data.data_out <= x"ee";
                else
					 	 dma_what_to_reply <= dma_last_reply;
							end if;
							
								case tx_count(15 downto 0) is
									when x"0000" => 
											udp_tx_int.data.data_out <= dma_what_to_reply;
									when x"0001" => 
											udp_tx_int.data.data_out <= dma_reply_additional_information(23 downto 16);
									when x"0002" => 
											udp_tx_int.data.data_out <= dma_reply_additional_information(15 downto 8);
									when x"0003" => 
											udp_tx_int.data.data_out <= dma_reply_additional_information(7 downto 0);
									when others =>    

					 
                  case tx_count(1 downto 0) is
                    when "00" =>
                        udp_tx_int.data.data_out <= fifo_dma_dout(31 downto 24);

                    when "01" =>
                        udp_tx_int.data.data_out <= fifo_dma_dout(23 downto 16);

                    when "10" =>
                        udp_tx_int.data.data_out <= fifo_dma_dout(15 downto 8);

                      -- it takes one clock cycle to get a new word from the fifo
                      if ( tx_count < tx_count_target-2 and fifo_dma_empty = '0' ) then
                        set_fifo_dma_read_next <= '1';
                      end if;

                    when "11" =>
                        udp_tx_int.data.data_out <= fifo_dma_dout(7 downto 0);

                    when others =>
                      udp_tx_int.data.data_out <= (others => '0');
                  end case;
                end case;

              when do_timeout =>
                if ( tx_count = 0 ) then
                  udp_tx_int.data.data_out <= do_timeout;
                else
                  udp_tx_int.data.data_out <= (others => '0');
                end if;

              when others =>    -- don't know what to do
                udp_tx_int.data.data_out <= do_error;
            end case;
          end if;

          -- the UDP module is ready to send data
          udp_tx_int.data.data_out_valid <= udp_tx_data_out_ready_int;
          if udp_tx_data_out_ready_int = '1' then

            -- we have reached the number of data bytes
            -- we wanted to send, lets finish
            if ( tx_count = tx_count_target-1 or
              ( what_to_do = do_read_dma and fifo_dma_empty = '1' and tx_count(1 downto 0) = "11" ) ) then
              set_last <= '1';
              set_tx_fin <= SET;
      --        set_tx_started <= CLR;
              next_state <= FINISH_SEND;
              set_state <= '1';
              test_display_int <= '1';
            else
              set_tx_count <= INCR;
            end if;

          end if;
        end if;


      when FINISH_SEND =>
        case what_to_do is
          when do_write_register =>
            reset_register_access <= '1';
				next_state <= IDLE;
				set_state <= '1';
			   reset_what_to_do <= '1';
			 when do_read_available_data =>
				reset_register_access <= '1';
				next_state <= IDLE;
				set_state <= '1';
			   reset_what_to_do <= '1';

          when do_read_register =>
            reset_register_access <= '1';
				next_state <= IDLE;
	   	   set_state <= '1';
		      reset_what_to_do <= '1';

          when do_read_dma =>
            reset_register_access <= '1';
            reset_dma_access <= '1';
			   if (more_than_one_pkg = '1') then
					next_state <= PREPARE_NEXT_PKG;
					set_state <= '1';
					reset_what_to_do <= '1';
--					debug_signal_0 <= '1';
         	else
					next_state <= IDLE;
					set_state <= '1';
					reset_what_to_do <= '1';
--				   debug_signal_0 <= '0';
				end if;

          when others =>
		   		next_state <= IDLE;
			      set_state <= '1';
			      reset_what_to_do <= '1';

        end case;

			when PREPARE_NEXT_PKG =>
--				udp_tx_int.data.data_out_valid <= '0';

           if (nextpkg_count > x"50") then
					set_tx_fin <= CLR;
					set_rx_count <= RST;
					set_nextpkg_count <= RST;
					set_tx_count <= RST;
					set_waiting_for_write_count <= RST;
					set_timeout_for_waiting_count <= RST;					
               
					set_what_to_do_nextpkg <= '1';

					next_state <= PREPARE_NEXT_PKG_2;
					set_state <= '1';
					
					set_dma_reply_additional_information <= INCR;
--					debug_signal_1 <= '0';
				else 
					set_nextpkg_count <= INCR;
--					debug_signal_1 <= '1';
					set_dma_reply_additional_information <= HOLD;
				end if;
			
			when PREPARE_NEXT_PKG_2 =>
					set_dma_reply_additional_information <= HOLD;
            if (nextpkg_count > x"02") then
					set_nextpkg_count <= RST;
   				set_next_register_read_dma_cnt <= '1';
					keep_register_addr <= '1';
					next_state <= WAIT_DATA_OUT_RDY;
					set_state <= '1';
--					debug_signal_2 <= '0';
				else 
--					debug_signal_2 <= '1';
					set_nextpkg_count <= INCR;
				end if;


    end case;
  end process;



  -- TX response process - SEQ
  tx_proc_sequential: process(tx_mac_aclk)
  begin
    if rising_edge(tx_mac_aclk) then
      if glbl_rst = '1' then
        -- reset state variables
        state <= IDLE;
        rx_count <= x"00";				
		  nextpkg_count <= x"00";
        tx_count <= x"0000";
        waiting_for_write_count <= x"000";
        timeout_for_waiting_count <= x"00";
        rx_pkg_ctr_int <= x"00000000";
        tx_start_reg <= '0';
        tx_hdr.dst_ip_addr <= (others => '0');
        tx_hdr.dst_port <= (others => '0');
        tx_hdr.src_port <= (others => '0');
        tx_hdr.data_length <= (others => '0');
        tx_hdr.checksum <= (others => '0');
       -- tx_started_reg <= '0';
        tx_fin_reg <= '0';

        what_to_do <= (others => '0');
        register_access_int <= '0';
        register_addr_int <= (others => '0');
        register_write_or_read_int <= '0';
        register_write_data_int <= (others => '0');
        -- register_read_data_int <= (others => '0');
        register_dma_int <= '0';
        register_dma_count_int <= (others => '0');
     --   register_dma_is_empty <= '0';
      else

        -- Next rx_state processing
        if set_state = '1' then
          state <= next_state;
        else
          state <= state;
        end if;

        -- count processing
		 case set_nextpkg_count is
			when RST =>  		nextpkg_count <= x"00";
			when INCR => 		nextpkg_count <= nextpkg_count + 1;
			when HOLD => 		nextpkg_count <= nextpkg_count;
		 end case;
		 case set_dma_reply_additional_information is
			when RST =>  		dma_reply_additional_information <= x"000000";
			when INCR => 		dma_reply_additional_information <= dma_reply_additional_information + 1;
			when HOLD => 		dma_reply_additional_information <= dma_reply_additional_information;
		 end case;

        case set_rx_count is
          when RST =>     rx_count <= x"00";
          when INCR =>    rx_count <= rx_count + 1;
          when HOLD =>    rx_count <= rx_count;
        end case;
        case set_tx_count is
          when RST =>     tx_count <= x"0000";
          when INCR =>    tx_count <= tx_count + 1;
          when HOLD =>    tx_count <= tx_count;
        end case;
        case set_waiting_for_write_count is
          when RST =>     waiting_for_write_count <= x"000";
          when INCR =>    waiting_for_write_count <= waiting_for_write_count + 1;
          when HOLD =>    waiting_for_write_count <= waiting_for_write_count;
        end case;
        case set_timeout_for_waiting_count is
          when RST =>     timeout_for_waiting_count <= x"00";
          when INCR =>    timeout_for_waiting_count <= timeout_for_waiting_count + 1;
          when HOLD =>    timeout_for_waiting_count <= timeout_for_waiting_count;
        end case;
        case set_pkg_count is
          when RST =>     rx_pkg_ctr_int <= x"00000000";
          when INCR =>    rx_pkg_ctr_int <= rx_pkg_ctr_int + 1;
          when HOLD =>    rx_pkg_ctr_int <= rx_pkg_ctr_int;
        end case;

        -- set tx hdr
        if set_hdr = '1' then
          tx_hdr.dst_ip_addr <= udp_rx_int.hdr.src_ip_addr; -- reply to sender
          tx_hdr.dst_port <= udp_rx_int.hdr.src_port;
          tx_hdr.src_port <= udp_rx_int.hdr.dst_port;
          tx_hdr.data_length <= std_logic_vector(to_unsigned(tx_count_target, tx_hdr.data_length'length));

          -- todo
          tx_hdr.checksum <= x"0000";
        else
          tx_hdr <= tx_hdr;
        end if;

        -- set tx start signal
        case set_tx_start is
          when SET  => tx_start_reg <= '1';
          when CLR  => tx_start_reg <= '0';
          when HOLD => tx_start_reg <= tx_start_reg;
        end case;

        -- set tx started signal
     --   case set_tx_started is
     --     when SET  => tx_started_reg <= '1';
     --     when CLR  => tx_started_reg <= '0';
     --     when HOLD => tx_started_reg <= tx_started_reg;
     --   end case;

        -- set tx finished signal
        case set_tx_fin is
          when SET  => tx_fin_reg <= '1';
          when CLR  => tx_fin_reg <= '0';
          when HOLD => tx_fin_reg <= tx_fin_reg;
        end case;

        -- what should we do?
        if ( set_what_to_do = '1' ) then
          what_to_do <= udp_rx_int.data.data_in;

          -- what else is defined by this selection?
          -- first default values
          tx_count_target <= 4;
          register_write_or_read_int <= '0';

          -- now case independent
          case udp_rx_int.data.data_in is
            when do_read_register =>
              tx_count_target <= 8;
            when do_write_register =>
              register_write_or_read_int <= '1';
            when do_pkg_count_read =>
              tx_count_target <= 8;
			--	when do_read_available_data =>
			--	  tx_count_target <=4;
            when do_read_dma =>
              --tx_count_target <= 16;  -- this has to be dynamic
              register_dma_int <= '1';
            when others =>
          end case;
        end if;
		  
		  --next pkg from dma read
		  if (set_what_to_do_nextpkg = '1') then
			what_to_do <= do_read_dma;
			register_write_or_read_int <= '0';
			register_dma_int <= '1';
		  end if;

		  
        if ( reset_what_to_do = '1' ) then
          what_to_do <= (others => '0');
        end if;

        -- there was a timeout, modify what to send
        if ( set_timeout_for_waiting_exeeded = '1' ) then what_to_do <= do_timeout; end if;

        -- set some stuff for register handling
        if ( set_register_addr1  = '1' )    then register_addr_int(15 downto 8) <= udp_rx_int.data.data_in; end if;
        if ( set_register_addr2  = '1' )    then register_addr_int(7 downto 0) <= udp_rx_int.data.data_in; end if;
        if ( set_register_addr_zero  = '1' )  then register_addr_int <= x"0000"; end if;
        if ( set_register_write_data1 = '1' ) then register_write_data_int(31 downto 24)  <= udp_rx_int.data.data_in; end if;
        if ( set_register_write_data2 = '1' ) then register_write_data_int(23 downto 16)  <= udp_rx_int.data.data_in; end if;
        if ( set_register_write_data3 = '1' ) then register_write_data_int(15 downto 8) <= udp_rx_int.data.data_in; end if;
        if ( set_register_write_data4 = '1' ) then register_write_data_int(7 downto 0)  <= udp_rx_int.data.data_in; end if;
        if ( set_register_read_dma_cnt1 = '1' )	then register_dma_count_int(31 downto 24)	<= udp_rx_int.data.data_in; end if;
		  if ( set_register_read_dma_cnt2 = '1' )	then register_dma_count_int(23 downto 16)		<= udp_rx_int.data.data_in; end if;
		  if ( set_register_read_dma_cnt3 = '1' )	then register_dma_count_int(15 downto 8)	<= udp_rx_int.data.data_in; end if;
		  if ( set_register_read_dma_cnt4 = '1' )	then register_dma_count_int(7 downto 0)	<= udp_rx_int.data.data_in; end if;
        if ( set_register_access = '1' )    then register_access_int <= '1'; end if;
      --  if ( unset_register_access = '1' )    then register_access_int <= '0'; end if;
        --if ( set_register_read_data = '1' )   then register_read_data_int <= REGISTER_READ_DATA; end if;
		  if ( set_next_register_read_dma_cnt = '1' )	then register_dma_count_int <= how_many_left; end if;
		  if ( keep_register_addr  = '1' )		then register_addr_int <= remember_addr; end if;


        if ( set_register_dma_count = '1' ) then
          register_write_data_int(31 downto 0) <= (others => '0');
          --register_write_data_int(1 downto 0) <= "00";
          --register_write_data_int(17 downto 2) <= std_logic_vector( to_unsigned(3,16) );
			 more_than_one_pkg <= '0';


          if ( register_dma_count_int > 367 and REGISTER_DMA_COUNT > 367 ) then -- max: 367 32-bit-words per UDP
            register_write_data_int(17 downto 2) <= std_logic_vector( to_unsigned(367,16) );
            tx_count_target <= 368*4;
 			if(register_dma_count_int > 23121) then 
					how_many_left <= "00000000000000000101100011100010";
			else
				how_many_left <= register_dma_count_int - 367;
			end if;
				remember_addr <= register_addr_int;
				more_than_one_pkg <= '1';
        
			 elsif ( REGISTER_DMA_COUNT = 0 ) then -- or REGISTER_DMA_COUNT = 0 ) then
            register_write_data_int(17 downto 2) <= (others => '0');
            tx_count_target <= 4;
			 elsif ( register_dma_count_int > REGISTER_DMA_COUNT ) then
            register_write_data_int(10 downto 2) <= REGISTER_DMA_COUNT(8 downto 0);
            tx_count_target <= to_integer(unsigned(REGISTER_DMA_COUNT + 1)) *4;
          elsif (register_dma_count_int=0) then
            register_write_data_int(17 downto 2) <= (others => '0');
            tx_count_target <= 4;	
 
          else
            register_write_data_int(10 downto 2) <= register_dma_count_int(8 downto 0);
            tx_count_target <= to_integer(unsigned(register_dma_count_int +1)) *4;
          end if;
        end if;

        fifo_dma_read_en <= set_fifo_dma_read_next;

        if ( reset_register_access = '1' ) then
          register_access_int <= '0';
          register_addr_int <= (others => '0');
          register_write_or_read_int <= '0';
          register_write_data_int <= (others => '0');
          -- register_read_data_int  <= (others => '0');
        end if;

        if ( reset_dma_access = '1' ) then
          register_dma_int <= '0';
          register_dma_count_int <= (others => '0');

        end if;

      end if;
    end if;


  end process;

  -- Make a copy of the register_access_int signal to prevent strange
  -- behavior between the two clock domains
  register_access_clock_transfer : process ( REGISTER_CLK )
  begin
    if rising_edge( REGISTER_CLK ) then
      register_access_clk_sync <= register_access_int;
    end if;
  end process;

  register_output : process ( REGISTER_CLK ) --, register_access_int, register_dma_int )
  begin
    if rising_edge( REGISTER_CLK ) then
      register_access_delayed <= register_access_clk_sync;
      register_read_ready_delayed <= REGISTER_READ_READY;
      -- register_dma_empty_delayed <= REGISTER_DMA_EMPTY;
      --register_access_delayed_2 <= register_access_delayed;

      if ( register_access_clk_sync = '1' ) then
        REGISTER_ADDR <= register_addr_int;
        REGISTER_WRITE_DATA <= register_write_data_int;
        REGISTER_WRITE_OR_READ <= register_write_or_read_int;
        REGISTER_DMA <= register_dma_int;

        -- Without this wait the reading of the register value
        -- would happen just on the edge. Sometimes this leads
        -- to wrong values.
        -- Furthermore we do not set REGISTER_ACCESS with DMA mode
        REGISTER_ACCESS <= not register_dma_int and register_access_clk_sync and not register_access_delayed;

        --if ( register_dma_int = '0' ) then

          -- if ( wait_with_register_access = '0' ) then
          --   wait_with_register_access <= '1';
          -- else
          --   REGISTER_ACCESS <= '1';
          -- end if;

        -- We also have to do something about the DMA access.
        -- The access signal only needs to be active for
        -- one clock cycle.
        --else
        --  REGISTER_ACCESS <= '0';
          -- if ( register_dma_access_init = '1' ) then
          --   REGISTER_ACCESS <= '0';
          -- else
          --   REGISTER_ACCESS <= '1';
          --   register_dma_access_init <= '1';
          -- end if;
        --end if;

        if ( REGISTER_READ_READY = '1' and register_read_ready_delayed = '0' ) then
          register_read_data_int <= REGISTER_READ_DATA;
        end if;
      else
        -- if ( wait_with_register_access = '1' ) then
        --  wait_with_register_access <= '0';
        -- else
        -- if ( register_access_delayed = '0' ) then
          REGISTER_ADDR <= (others => '0');
          REGISTER_WRITE_OR_READ <= '0';
          REGISTER_WRITE_DATA <= (others => '0');
          REGISTER_DMA <= '0';
          register_read_data_int <= (others => '0');
        -- end if;
        REGISTER_ACCESS <= '0';
        -- register_dma_access_init <= '0';
      end if;
    end if;

    -- if ( register_access_int = '1' and register_dma_int = '1' and finish_get_dma_data = '1' ) then
    --   REGISTER_ACCESS <= '0';
    --   --REGISTER_DMA <= '0';
    --   --REGISTER_WRITE_DATA(31) <= '1';
    -- end if;
  end process;


  ------------------------------------------------------------------------------
  -- Instantiate the FIFO for buffering the DMA block transfer data
  ------------------------------------------------------------------------------
  udp_block_data_fifo : entity work.udp_block_data_fifo
  PORT MAP (
    RST     => fifo_dma_reset,
    WR_CLK  => REGISTER_CLK,
    RD_CLK  => GTX_CLK_BUFG,
    DIN     => fifo_dma_din,
    WR_EN   => fifo_dma_write_en,
    RD_EN   => fifo_dma_read_en,
    DOUT    => fifo_dma_dout,
    FULL    => fifo_dma_full,
    EMPTY   => fifo_dma_empty,
	 RD_DATA_COUNT => fifo_data_count
  );

  -- get the DMA block data and transfer it to the UDP block data fifo
  get_dma_data : process (
    glbl_rst, reset_dma_access, REGISTER_READ_READY, set_what_to_do,
    fifo_dma_transfer_end, REGISTER_DMA_END, REGISTER_DMA_EMPTY, fifo_dma_full )
  begin
    fifo_dma_reset <= reset_dma_access or glbl_rst;
    fifo_dma_din <= REGISTER_READ_DATA;
    REGISTER_DMA_WAIT <= fifo_dma_full;-- or finish_get_dma_data;

    fifo_dma_write_en <= register_dma_int and REGISTER_READ_READY and not finish_get_dma_data;
        -- (
        --   (REGISTER_READ_READY and not finish_get_dma_data) or
        --   (REGISTER_DMA_EMPTY and register_read_ready_delayed)
        -- );

    fifo_dma_transfer_end <= REGISTER_DMA_END or fifo_dma_full; --REGISTER_DMA_EMPTY or fifo_dma_full;

    finish_get_dma_data <= ( finish_get_dma_data or fifo_dma_transfer_end ) and not (set_what_to_do or set_what_to_do_nextpkg);


    -- if ( fifo_dma_transfer_end = '1' ) then
    --   finish_get_dma_data <= '1';
    -- elsif ( fifo_dma_reset = '1' ) then
    --   finish_get_dma_data <= '0';
    -- -- else
    -- --   finish_get_dma_data <= finish_get_dma_data;
    -- end if;
  end process;

end Behavorial;
