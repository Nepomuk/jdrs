-----------------------------------------------------------------------------
--
--                         Wilhelm Erven  |       ######
-- Zentralinstitut fuer Elektronik (ZEL)  |    #########
--        Forschungszentrum Juelich GmbH  |   #########   ##
--              D-52425 Juelich, Germany  |  #########   ####
--                                        |  ########   #####
--             (+49)2461 61 4130 :   Tel  |   #   ##   #####
--             (+49)2461 61 3573 :   FAX  |    ##     #####
--         w.erven@fz-juelich.de : EMail  |       ######
--
-----------------------------------------------------------------------------
-- ==========================================================================
--
--	project:		DAQ
--	author:		W.Erven ZEL.FZJ
--
-- History
-- Date     | Rev | Author    | What
------------+-----+-----------+----------------------------------------------
--	06.04.09 | 0.1 | W.Erven   | start of the desing
--
-- ==========================================================================
--
--	file:		sm_pack.vhd
--
-- package:	sample_package
--
-- ==========================================================================
--
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;
use work.util_pack.all;

package sample_package is

---------------------------------------------------------------------------
-- constants for select GIGALink register
---------------------------------------------------------------------------


	subtype T_2SL		is std_logic_vector(1 downto 0);
	subtype T_SLV7		is std_logic_vector(6 downto 0);
	subtype T_SLV10	is std_logic_vector(9 downto 0);
	subtype T_SLV12	is std_logic_vector(11 downto 0);
	subtype T_SLV17	is std_logic_vector(16 downto 0);
	subtype T_SLV20	is std_logic_vector(19 downto 0);
	subtype T_SLV22	is std_logic_vector(21 downto 0);
	subtype T_SLV24	is std_logic_vector(23 downto 0);
	subtype T_SLV26	is std_logic_vector(25 downto 0);
	subtype T_SLV33	is std_logic_vector(32 downto 0);
	subtype T_SLV34	is std_logic_vector(33 downto 0);

	subtype T_PADR		is std_logic_vector(12 downto 2);
	subtype T_HADR		is std_logic_vector(8 downto 2);
	---------------------------------------------------------------------------
	-- constants for select GIGALink register
	---------------------------------------------------------------------------

	constant SC_VERSION		: integer :=16#10010105#;		-- entsp. SIS spec.
													-- FirmwRev/FirmwID/HardwRev/BoardID
	constant SM_VERSION_HW	: integer :=16#08#;		-- hardware version 08.0
	constant SM_VERSION_FW	: integer :=16#02#;		-- firmware version 0.2

	---------------------------------------------------------------------------
	-- constants for select GIGALink register
	---------------------------------------------------------------------------

	constant GLS_IDENT				: integer	:=16#00#; --  alle werte mal vier nehmen..........
	constant GLS_STATUS_REGISTER	: integer	:=16#01#;  --
	constant GLS_CONTROL_REGISTER : integer   :=16#02#; -- 0x8

-- "0011" ist fuer semaphore reserviert!
	constant GLS_DCOUNT				: integer	:=16#04#;
	constant GLS_BLKSZ				: integer	:=16#05#;
	constant GLS_COUNTER_DRP		: integer	:=16#06#;  -- 018
	constant GLS_DOORBELL_MASK		: integer	:=16#07#;  -- 008
	constant SM_DMA_CONTROL			: integer 	:=16#08#; 	-- 410   Daq control (bis jetzt startet es nur das fuellen mit dummy daten)
																			-- kann (sollte) man aber auch ins GLS_CONTROL uebernehmen


	constant GLS_TIMER				: integer	:=16#08#; 	--hex20
	
-- Adressbereich fuer MMCM_DRP
	constant SM_DRP					: integer	:=16#20#;	--  to 16#3F#, 32 regs	

-- ======================================================================= --
-- constants for the regs                              --
-- ======================================================================= --

	constant RB_BUSY_REG 			: integer	:=16#104#; -- hex address 410 
	constant RB_DCMDATA_REG			: integer	:=16#105#;	--hex address 414
	
--	constant TPX_FIFODUMMY_REG  	: T_MGLREG	:="10101";	--hex address 854 (adresse wurde geaendert, zurueckaendern! )
	constant TPX_FIFODUMMY_REG  	: integer	:=16#108#;  --hex address 420
	constant TPX_LEDINFOREG			: integer	:=16#107#;  --hex address 41c
	constant TPX_SLOW_CTRL		 	: integer	:=16#120#;  --hex address 480
	constant TPX_INPUT_REG		 	: integer	:=16#121#;	--hex address 484
	constant TPX_INPUTCOUNT_REG 	: integer  	:=16#122#;	--hex address 488
	constant TPX_TRIGCOUNT_REG		: integer	:=16#124#;  --hex address 490

	constant LTCREG_SEL			 	: integer 	:=16#125#;	--hex address 494

	constant TPXMASTER_BUSSEL1	 	: integer	:=16#126#;	--hex address 498
	constant TPXMASTER_BUSSEL0	 	: integer	:=16#127#;	--hex address 49C
	constant TPXMASTER_APRECHRG 	: integer	:=16#128#;	--hex address 4A0
	constant TPXMASTER_DPRECHRG 	: integer	:=16#129#;	--hex address 4A4
	constant TPXMASTER_ALATCH	 	: integer	:=16#12a#;	--hex address 4A8
	constant TPXMASTER_DLATCH	 	: integer	:=16#12b#;	--hex address 4AC
	constant TPXMASTER_READCMD	 	: integer	:=16#12c#;	--hex address 4B0
	constant TPXMASTER_READLE	 	: integer	:=16#12d#;	--hex address 4B4
	constant TPXMASTER_READTE	 	: integer	:=16#12e#;	--hex address 4B8
	constant TPXMASTER_TRIGGERLO 	: integer	:=16#12f#;	--hex address 4BC
	constant TPXMASTER_TRIGGERHI 	: integer	:=16#130#;	--hex address 4C0
	constant TPXMASTER_BITCOUNT 	: integer	:=16#131#;	--hex address 4C4	
	
	constant LED_REG					: integer 	:=16#132#; 	--hex address 4C8 
	
	constant LCD_MODE_REG			: integer 	:=16#133#; 	--hex address 4CC 
	constant LCD_FIFO_CHAR_REG		: integer 	:=16#134#; 	--hex address 4D0 
--	constant LCD_CHAR_REG_1_1		: integer 	:=16#134#; 	--hex address 4D0 
--	constant LCD_CHAR_REG_1_2		: integer 	:=16#135#; 	--hex address 4D4
--	constant LCD_CHAR_REG_1_3		: integer 	:=16#136#; 	--hex address 4D8
--	constant LCD_CHAR_REG_1_4		: integer 	:=16#137#; 	--hex address 4DC
--	constant LCD_CHAR_REG_2_1		: integer 	:=16#138#; 	--hex address 4E0 
--	constant LCD_CHAR_REG_2_2		: integer 	:=16#139#; 	--hex address 4E4
--	constant LCD_CHAR_REG_2_3		: integer 	:=16#13a#; 	--hex address 4E8
--	constant LCD_CHAR_REG_2_4		: integer 	:=16#13b#; 	--hex address 4EC      
	
--	constant SM_IDENT					: integer :=16#101#;  	-- 404   raus, doppelt zu GLS_IDENT
	
	constant SCR_ENA					: integer := 0;
	constant SCR_HIGH					: integer := 15;

	constant SM_SR						: integer :=16#132#;    --
	constant SM_BLK_SIZE				: integer :=16#106#;    -- 418
	constant SM_RO_DATA				: integer :=16#0100#;   -- Register Adresse fuer DMA transfer

-- -------------------------- Giga Link protocol constants --------------- --

	constant GL_START	: T_SLV8 := x"1C";	-- K.28.0  
	constant GL_XON	: T_SLV8 := x"5C";	-- K.28.2  
	constant GL_XOFF	: T_SLV8 := x"7C";	-- K.28.3  
	constant GL_RESET	: T_SLV8 := x"9C";	-- K.28.4  
	constant GL_IDLE	: T_SLV8 := x"BC";	-- K.28.5  
	constant GL_SYNC	: T_SLV8 := x"DC";	-- K.28.6  

end sample_package;

-- ======================================================================= --

package body sample_package is

end sample_package;
