-- -----------------------------------------------------------------------------
--
--                           Willi Erven  |       ######
-- Zentralinstitut fuer Elektronik (ZEL)  |    #########
--        Forschungszentrum Juelich GmbH  |   #########   ##
--              D-52425 Juelich, Germany  |  #########   ####
--                                        |  ########   #####
--             (+49)2461 61 4130 :   Tel  |   #   ##   #####
--             (+49)2461 61 3573 :   FAX  |    ##     #####
--         w.erven@fz-juelich.de : EMail  |       ######
--
-- -----------------------------------------------------------------------------
-- =============================================================================
--
--	project:		WASA
--	module:		sysctrl => system controller
--	author:		W.Erven ZEL.FZJ
--
-- History
-- Date     | Rev | Author    | What
-- ---------+-----+-----------+-------------------------------------------------
-- 19.07.02 | 0.0 | W.Erven   | begin of new DAQ system development

-- ======================================================================= --
--
--	timing
-- ilclk    -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
-- P_REG   ____------____  ____--------____
-- P_WR    ____------____  ________________
-- P_A     0===x=====0===  0===x=======0===
-- P_D     0===x=====0===
-- P_D_O                   0===x=d=====0===
-- P_RDY   ________--____  ______--....____
--
-- ======================================================================= --
--
library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_unsigned.all;

use work.util_pack.all;
use work.sample_package.all;

Library UNISIM;
use UNISIM.vcomponents.all;

entity SREGS is
generic (
	SC_VERSION	: integer
);
port (
	LCLK				: out	std_logic;
	BASECLOCK		: in   	std_logic;	-- 125 MHz
	CLK66				: in    std_logic; 	-- 66 MHz
	GRESET   		: out	std_logic;	-- general reset
	P1MS				: in	std_logic;	-- 1 ms pulse
	LED				: out 	std_logic_vector(7 downto 0);
	USER_SWITCH		: in	std_logic_vector(4 downto 0);
--	-------------------------- local bus zum GIGALink Teil ---------------- --
	P_REG				: in	std_logic;								-- Signal fuer Registerzugriffe
	P_WR				: in	std_logic;								-- write
	P_A				: in	std_logic_vector(12 downto 2);	-- address
	P_D				: in	T_SLV32;									-- data - byte count is swapped
	P_D_O				: out	T_SLV32;									-- data out
	P_RDY				: out	std_logic;								-- data ready
	P_BLK				: in    std_logic;								-- always read
	P_WAIT			: in 	std_logic;								--	pause block read
	P_END				: out 	std_logic;								-- block end
	--	-------------------------- control signals ---------------------------- --
	DMD_DMA			: in	std_logic;
	EV_DATACOUNT	: out   std_logic_vector(16 downto 0);
	DT_REQ			: out	std_logic;
	DT_ACK			: in	std_logic;
	DT_DEN			: out	std_logic;		-- data enable on PD
	FIFO_EMPTY  	: out 	std_logic;
--	-------------------------- write to Host register request ------------- --
	HREG_REQ			: out	std_logic;
	HREG_A			: out	std_logic_vector(8 downto 2);		-- address
	HREG_D			: out	T_SLV32;
	HREG_ACK			: in	std_logic;
-- -------------------------- control/status ----------------------------- --
	SYS_MODE			: out	T_SLV16;		-- control
-- -------------------------- host doorbell ------------------------------ --
	P100MS			: in	std_logic;		-- 100 ms signal
	DMD_WR			: in	std_logic		-- write demand data to host
);
end SREGS;

architecture RTL of SREGS is

-- ======================================================================= --
--										    Components                               --
-- ======================================================================= --

COMPONENT one_hertz_counter
PORT(
	CLK 				: IN std_logic;
	COUNTER_OUT 		: OUT std_logic_vector(7 downto 0)
	);
END COMPONENT;
component knight_rider
	port (
		CLK66			: in	std_logic;
		USER_SWITCH		: in	std_logic_vector (4 downto 0);
		LED				: out	std_logic_vector (7 downto 0)
	);
end component;

component sm_dummy_data is
Port (
	START 				: in  	STD_LOGIC;
	RESET 				: in 	std_logic;
	EMPTY 				: out 	std_logic;
	FULL				: out 	std_logic;
	DATA_COUNT			: out 	std_logic_vector (9 downto 0);
	CLK					: in 	std_logic;
	DIN 				: in  	STD_LOGIC_VECTOR (31 downto 0);
	DWEN 				: in 	std_logic ;
	DOUT 				: out  	STD_LOGIC_VECTOR (31 downto 0);
	DDESTWE 			: out  	std_logic;
	BUSY 				: out  	STD_LOGIC
	);
end component sm_dummy_data;

COMPONENT daq_fifo
  PORT (
    clk 		: 		IN STD_LOGIC;
    srst 		: 		IN STD_LOGIC;
    din 		: 		IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    wr_en 		: 		IN STD_LOGIC;
    rd_en 		: 		IN STD_LOGIC;
    dout 		: 		OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    full 		: 		OUT STD_LOGIC;
    empty 		: 		OUT STD_LOGIC;
    prog_full 	: 		OUT STD_LOGIC;
	 data_count	: 		out std_logic_vector(16 downto 0)
  );
END COMPONENT;

-- ======================================================================= --
--										    Signals                                  --
-- ======================================================================= --
	alias regadr						: std_logic_vector(8 downto 0) is P_A(10 downto 2);
-- control/status register
	signal doorbell_mask				: T_SLV16;
	signal stimer						: T_SLV32;
	signal stimer_clr					: std_logic;
-- block counter
	signal dmd_counter					: std_logic_vector(31 downto 2);
	signal dmd_block_sz					: std_logic_vector(23 downto 2);
	signal dmd_block					: std_logic_vector(23 downto 2);
	signal dmd_load						: std_logic;
	signal dmd_int						: std_logic;
-- host doorbell
	signal cnt1sec						: T_SLV4;
	signal s2sec						: std_logic;
	signal dbell_in						: T_SLV32;
	signal dbell_in_d					: T_SLV32;
	signal dbell_act					: T_SLV32;
	signal dbell_out					: T_SLV32;
	signal dbell_req					: std_logic;
	signal creg							: T_SLV32;
	signal tpx_fifodummyreg     		: std_logic_vector(31 downto 0);
	signal tpx_fifodummydata     		: std_logic_vector(31 downto 0);
	signal tpx_fifo_din					: std_logic_vector(31 downto 0);
	signal tpx_fifo_datacount			: std_logic_vector(9 downto 0);
	signal tpx_fifodummywe				: std_logic;
	signal synthieclock         		: std_logic;
	signal dummy_data_busy				: std_logic;
	signal tpx_fifodummydestwe			: std_logic;
	signal clock_counter				: std_logic_vector(11 downto 0):= "000000000000";
	signal led_config					: std_logic_vector(4 downto 0);
	signal one_hertz_counter_i			: std_logic_vector(7 downto 0);
	signal knight_rider_i				: std_logic_vector(7 downto 0);
	signal ilclk,ilreset				: std_logic;

-- ========================== MMCM_DRP ====================================== --
	signal rc_do						: T_SLV16;
	signal rc_we,rc_start				: std_logic;
	signal drp_busy						: std_logic;
	signal s_rdy						: std_logic;		-- single ready (no block)
	signal rdy_fi,rdy_fi_d1				: std_logic;
	signal fi_count,fi_cnt_max			: T_SLV10;
	signal fi_wen						: std_logic;
	signal fi_ren						: std_logic;
	signal fi_empty,fi_full				: std_logic;
	signal fi_pfull						: std_logic;		-- 3/4
	signal fi_datacount					: std_logic_vector(16 downto 0);
	signal fi_din,fi_dout, fi_din_dummy	: std_logic_vector(31 downto 0);
	signal fi_valid						: std_logic;
	signal fill_dma_counter   : std_logic_vector(31 downto 0);
	signal blk							: std_logic;
	signal blk_rdy						: std_logic;
	signal blk_cnt						: std_logic_vector(17 downto 0);	-- lword count, 1MB
	signal blk_zero						: std_logic;	-- zero
	signal ldt_den						: std_logic;

-- ======================================================================= --
--										       Begin                                 --
-- ======================================================================= --

begin

-- ======================================================================= --
--						            Components Port Map                          --
-- ======================================================================= --
LCLK		<= ilclk;
GRESET	<= ilreset;

U_MMCM: entity work.U_MMCM
port map (
	CLKIN			=> CLK66, --BASECLOCK, -- ilclk ist eigentlich baseclock
	LCLK			=> ilclk,  -- ilclk kommt aus mmcm raus
	LRESET		=> ilreset,
	BCLK			=> open,
	BRESET		=> open,   -- not locked
	RC_A			=> regadr(4 downto 0),
	RC_DO			=> rc_do,
	RC_DI			=> P_D(15 downto 0),
	RC_WE			=> rc_we,
	RC_START		=> rc_start,  --creg(5),
	RC_CLK		=> open,
	RC_CLK180	=> open,
	RC_RESET		=> drp_busy
);
rc_we		<= P_REG and P_WR and B2SL(regadr(regadr'high downto 5) = (SM_DRP/32));
rc_start	<= P_REG and P_WR and B2SL(regadr = (SM_DRP+31));

tpx_fifo_din <= tpx_fifodummyreg;

U_DUMMY_DATA :  sm_dummy_data
    Port  map(
		START 			=> creg(4),
		RESET			=> ilreset,
		EMPTY			=> open,
		FULL			=> open,
		DATA_COUNT 		=> tpx_fifo_datacount,
		CLK				=> ilclk,
		DIN 			=> tpx_fifo_din,
		DWEN			=> tpx_fifodummywe,
		DOUT 			=> tpx_fifodummydata,
		DDESTWE			=> tpx_fifodummydestwe,
		BUSY 			=> dummy_data_busy
	);

dma_buffer: daq_fifo
	port map(
		clk			=> ilclk,
		srst		=> ilreset,
		din			=> fi_din,
		wr_en		=> fi_wen,
		rd_en		=> fi_ren,
		dout		=> fi_dout,
		full		=> fi_full,
		empty		=> fi_empty,
		prog_full	=> fi_pfull,
		data_count	=> fi_datacount
	);

U_ONE_HERTZ_COUNTER : one_hertz_counter
	PORT MAP(
		CLK 		=> ilclk,
		COUNTER_OUT => one_hertz_counter_i
	);
U_KNIGHT_RIDER : knight_rider
	port map (
		CLK66		=> ilclk,
		USER_SWITCH	=> USER_SWITCH,
		LED			=> knight_rider_i
	);

with led_config select LED <=
	one_hertz_counter_i 				when "00000",		-- 0
	knight_rider_i (7 downto 0)		 	when "00001",		-- 1
	tpx_fifo_datacount(7 downto 0) 		when "00010",		-- 2
	fi_datacount(7 downto 0) 			when "00011",		-- 3
	"11111111" 							when others;


-- ======================================================================= --
--										Control Register and gen timer               --
-- ======================================================================= --

SYS_MODE	<= doorbell_mask;

-- ============================ Write to Register ======================== --
process(ilreset, stimer_clr, ilclk)
begin

	if ilclk'event and (ilclk = '1') then
	clock_counter <= clock_counter +1;


		if SL2B(P_REG and P_WR) and (regadr = GLS_CONTROL_REGISTER) then
			creg	<=P_D(CREG'range);
		end if;
		if SL2B(P_REG and P_WR) and (regadr = LED_REG) then
			led_config <= P_D(led_config'range);
		end if;

		-- if SL2B(P_REG and P_WR) and (regadr = TPX_FIFODUMMY_REG) then
			-- tpx_fifodummyreg <= P_D(tpx_fifodummyreg'range);
		-- end if;
		if dummy_data_busy ='1' then   -- start dummy data transfer from sm to data fifo
			creg(4) <='0';
		end if;
		if drp_busy ='1'  then
			creg(5)	<= '0';        --start Clockgen Reconfigure
		end if;


		if SL2B(P_REG and P_WR) and (regadr = TPX_FIFODUMMY_REG) then
			tpx_fifodummywe <= '1';
			tpx_fifodummyreg <= P_D(tpx_fifodummyreg'range);
		else
			tpx_fifodummywe <= '0';
		end if;

	if SL2B(ilreset) then
		creg						<= (others => '0');
		tpx_fifodummyreg 		<= (others => '0');
		led_config				<= (others => '0');
    end if;
	end if;
end process;

-- ------------------------------------------------------------------------- --
--                                fifodatamux                                --
-- ------------------------------------------------------------------------- --
process(ilclk)
begin
if rising_edge(ilclk) then
	if SL2B(P_REG and P_WR) and (regadr = GLS_DOORBELL_MASK) then
		doorbell_mask		<= P_D(doorbell_mask'range);
		doorbell_mask(1)	<= P_D(1) and not P_D(0); -- warum dieses konstruckt?? Auf (1) liegt fi_valid auf (0) dmd_init
		-- wenn P_D(0) = 0 (kein dma transfer) liegt an (1) an was auch auf 1 ankommt.
		-- wenn P_D(0) = 1  liegt an fi_valid immer eine 0 an. Es wird kein doorbell ueber neue Daten im Fifo gesendet.
		-- (weil die eh sofort ueber dma geholt werden)
	end if;

	stimer_clr	<= P_REG and P_WR and B2SL(regadr = GLS_TIMER);  -- stimer_clr setzt den Timer zurueck

--	if SL2B(P_REG and P_WR) and (regadr = GLS_CONTROL_REGISTER) then
--		creg				<= P_D(CREG'range);
--
--	end if;
		if SL2B(ilreset) then
		doorbell_mask		<= (others => '0');
    end if;

	if SL2B(ilreset or stimer_clr) then
		stimer	<= (others => '0');
	end if;

	if SL2B(P1MS) then
		stimer	<= stimer+1;
	end if;

end if;
end process;
--
-- ======================================================================= --
--										direct data counter                          --
-- ======================================================================= --
--
process(ilclk)
begin
if rising_edge(ilclk) then
	dmd_load			<= '0';
	if SL2B(P_REG and P_WR) and (regadr = GLS_BLKSZ) then
		dmd_block_sz	<= P_D(dmd_block_sz'range);
		dmd_load			<= '1';	-- pulse
	end if;

	if SL2B(ilreset) then
		dmd_block_sz	<= (others => '0');
	end if;

	if SL2B(DMD_WR) then
		dmd_counter	<= dmd_counter	+1;
	end if;

	if   SL2B(ilreset)
     or (SL2B(P_REG and P_WR) and (regadr = GLS_DCOUNT)) then
		dmd_counter	<= (others => '0');
	end if;

	if SL2B(DMD_WR) then
		dmd_block	<= dmd_block -1;
	end if;

	if   SL2B(dmd_load)
	  or (SL2B(DMD_WR) and (dmd_block = 0)) then
		dmd_block	<= dmd_block_sz;
	end if;

	dmd_int	<=    doorbell_mask(0) and DMD_WR
				  and B2SL((dmd_block_sz /= 0) and (dmd_block = 0));
end if;
end process;

-- ======================================================================= --
--										send to host doorbell                        --
-- ======================================================================= --
--
-- dbell_in   0===a===b===
-- dbell_in_d 0=====a===b===
-- dbell_act  ======a===c=====d=======0===
-- HREG_REQ   ________--------__------____
-- HREG_ACK   ______________--______--____
-- dbell_out  ========a=========d=======0=

dbell_in		<= x"00"							-- 24 free for status bits without interrupt
				  &x"0000"						--  8
				  &"0000"						--  4
				  &(doorbell_mask(3) and s2sec)		--  3
				  &(doorbell_mask(2) and fi_pfull)	--  2
				  &(doorbell_mask(1) and fi_valid)	--  1
				  &dmd_int;						--  0

process(ilclk)
begin
if rising_edge(ilclk) then
	if SL2B(P100MS) then
		cnt1sec	<= cnt1sec +1;
		if (cnt1sec = 10-1) then
			cnt1sec	<= (others => '0');
			s2sec		<= not s2sec;
		end if;
	end if;

	dbell_act		<=   (dbell_in and not dbell_in_d)
						  or (dbell_act and not (SL2SLV(HREG_ACK) and dbell_out));
	dbell_act(31 downto 24)	<= (others => '0');	-- don't send to doorbell, status bits only

	dbell_req	<=   (not dbell_req and B2SL(dbell_act /= 0))
					  or (dbell_req and not HREG_ACK);

	dbell_in_d	<= dbell_in;
	if SL2B(not dbell_req) then
		dbell_out	<= dbell_act;
	end if;

	if SL2B(ilreset) then
		dbell_act	<= (others => '0');
		dbell_req	<= '0';
	end if;
end if;

-- synopsys translate_off
if (cnt1sec(0) = 'U') then
	cnt1sec	<= (others => '0');
	s2sec		<= '0';
end if;
-- synopsys translate_on
end process;

HREG_REQ	<= dbell_req;
HREG_A	<= SL2SLV(dbell_req, HREG_A'length) and "0000100";	-- 16#0010# = doorbell
HREG_D	<= SL2SLV(dbell_req, HREG_D'length) and dbell_out;
--
-- ========================== concurrent assignments ===================== --
--

-- -------------------------- remote access ------------------------------ --
process(ilclk)
begin
if rising_edge(ilclk) then
  s_rdy   <=    P_REG
          and (     B2SL(P_A = SM_DMA_CONTROL )
             or   B2SL(P_A = SM_SR)
             or   B2SL(P_A = SM_BLK_SIZE)
             or   (B2SL(P_A = SM_RO_DATA) and not P_WR));

	rdy_fi		<= P_REG and not P_WR and B2SL(P_A = SM_RO_DATA);
	rdy_fi_d1	<= rdy_fi;

end if;
end process;

fi_ren	<= not fi_empty and (not fi_valid or blk_rdy or ldt_den or (rdy_fi_d1 and not rdy_fi));

blk_rdy	<=    blk and not blk_zero and not P_WAIT and fi_valid;

EV_DATACOUNT	<= fi_datacount +('0'&fi_valid);

ldt_den	<= DT_ACK and fi_valid;

DT_DEN	<= ldt_den;
P_END		<= blk and (blk_zero or not fi_valid);

DT_REQ	<= DMD_DMA and fi_valid;

-- ========================== process for Event Block Transfer =========== --
process(ilreset, P_BLK, ilclk)

begin
	if rising_edge(ilclk) then
		fi_valid		<=   fi_ren
						  or (fi_valid and not (blk_rdy or ldt_den or (rdy_fi_d1 and not rdy_fi)));

		if SL2B(ilreset) then
			fi_valid	<= '0';
		end if;

	end if;

	if SL2B(not P_BLK) then
		blk			<= '0';
	elsif ilclk'event and (ilclk = '1') then
		blk 			<= P_BLK and B2SL(P_A = SM_RO_DATA);
	end if;

	if rising_edge(ilclk) then
		if SL2B(blk_rdy) then
			blk_cnt	<= blk_cnt -1;
			if (blk_cnt(blk_cnt'high downto 1) = 0) then
				blk_zero	<= '1';
			end if;
		end if;

		if SL2B(P_BLK and not blk) then
			blk_zero		<= '0';
			blk_cnt		<= P_D(blk_cnt'high+2 downto 2);
			if (P_D(31 downto blk_cnt'high+3) /= 0) then
				blk_cnt		<= (others => '1');
			end if;
		end if;
	end if;
end process;

-- ========================================================================== --
--										sample data fifo											--
-- ========================================================================== --

-- fi_din <= tpx_fifodummydata;
-- fi_wen <= tpx_fifodummydestwe;

-- fill the DMA constantly with data
-- (comment the upper two lines out and the process below in)
fill_dma_with_counter : process( ilclk )
begin
	if rising_edge( ilclk ) then
		if SL2B(ilreset) then
			fill_dma_counter <= (others => '0');
		elsif ( fi_full = '1' ) then
			fill_dma_counter <= fill_dma_counter;
		else
			fill_dma_counter <= fill_dma_counter + 1;
		end if;
	end if;
end process;
fi_din <= fill_dma_counter;
fi_wen <= not fi_full;
-- fill DMA end

process(ilclk)
begin
if rising_edge(ilclk) then
	if SL2B(P_REG and P_WR) and (P_A = SM_BLK_SIZE) then
		fi_cnt_max	<= P_D(fi_cnt_max'range);
	end if;
	if SL2B(ilreset) then
		fi_cnt_max	<= (others => '0');
	end if;
end if;
end process;
-- ========================== concurrent assignments ===================== --
FIFO_EMPTY <= fi_empty;
-- ============================ Read from Register ======================= --
P_RDY	<= s_rdy or blk_rdy or
				(P_REG and (   (B2SL(regadr = GLS_IDENT) and not P_WR)
						  or B2SL(regadr = GLS_STATUS_REGISTER)
						  or B2SL(regadr = GLS_DOORBELL_MASK)
						  or B2SL(regadr = GLS_TIMER)
						  or B2SL(regadr = GLS_DCOUNT)
						  or B2SL(regadr = GLS_BLKSZ)
						  or B2SL(regadr = GLS_CONTROL_REGISTER)
						  or B2SL(regadr = RB_BUSY_REG)
						  or B2SL(regadr = RB_DCMDATA_REG)
						  or B2SL(regadr = TPX_FIFODUMMY_REG)
						  or B2SL(regadr = TPX_INPUTCOUNT_REG)
						  or B2SL(regadr = TPX_SLOW_CTRL)
						  or B2SL(regadr = TPX_INPUT_REG)
						  or B2SL(regadr = TPX_TRIGCOUNT_REG)
						  or B2SL(regadr = LED_REG)
						  or B2SL(regadr = TPX_LEDINFOREG)
						  or B2SL(regadr(regadr'high downto 5) = (SM_DRP/32)))		-- MMCM_DRP
						  );

P_D_O <=
			(    SL2SLV(P_REG and not P_WR and B2SL(regadr = GLS_IDENT))
			 and INT2SLV(SC_VERSION))
		or (    SL2SLV(P_REG and not P_WR and B2SL(regadr = GLS_STATUS_REGISTER))
			 and dbell_in)
		or (    SL2SLV(P_REG and not P_WR and B2SL(regadr = GLS_DOORBELL_MASK))
			 and EXT2SLV(doorbell_mask))
		or (    SL2SLV(P_REG and not P_WR and B2SL(regadr = GLS_TIMER))
			 and stimer)
		or (    SL2SLV(P_REG and not P_WR and B2SL(regadr = GLS_DCOUNT))
			 and dmd_counter&"00")
		or (    SL2SLV(P_REG and not P_WR and B2SL(regadr = GLS_BLKSZ))
			 and EXT2SLV(dmd_block_sz&"00"))
		or (    SL2SLV(P_REG and not P_WR and B2SL(regadr = GLS_CONTROL_REGISTER))
			 and creg)
		or (	  SL2SLV(P_REG and not P_WR and B2SL(regadr = LED_REG))
			 and EXT2SLV(led_config))
		or	(    SL2SLV(P_REG and not P_WR and B2SL(P_A = SM_SR))
			 and EXT2SLV(fi_pfull&fi_valid))
		or (    SL2SLV(P_REG and not P_WR and B2SL(P_A = SM_BLK_SIZE))
			 and EXT2SLV(fi_cnt_max))
		or (    SL2SLV(not fi_valid and s_rdy and not P_WR and B2SL(P_A = SM_RO_DATA))  -- P_REG -> s_rdy
			 and x"EEEEEEEE")
		or (    SL2SLV(fi_valid and (   blk or DT_ACK	or (s_rdy and not P_WR and B2SL(P_A = SM_RO_DATA)) ))  -- P_REG -> s_rdy
			 and fi_dout)
-- ========================== MMCM_DRP ====================================== --
		or (    SL2SLV(    P_REG and not P_WR
						   and B2SL(regadr(regadr'high downto 5) = (SM_DRP/32)))
			 and EXT2SLV(rc_do))
		 ;
end RTL;
