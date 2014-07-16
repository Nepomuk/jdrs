----------------------------------------------------------------------------------
-- Company:  IKP 1 FZ Juelich 
-- Engineer:  Simone Esch 
-- 
-- Create Date:    15:07:27 06/27/2011 
-- Design Name: 
-- Module Name:    sm_dummy_data - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description:	SM to save dummy data in a FIFO and transfer it on demand at once to somwhere else.
--						The dummy FIFO can be filled via writing a register. 
--
--						The FIFO is a First-Word-Fall-Through Fifo. No readout latency
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity sm_dummy_data is
    Port ( 	START 	: in  	STD_LOGIC; -- ein start, fuer transfer, gefuellt wird das fifo einfach ueber registerschreiben. 
				CLK		: in 		std_logic;
				RESET 	: in 		std_logic;
				EMPTY		: out 	std_logic;
				FULL		: out 	std_logic;
				DATA_COUNT :	out std_logic_vector(9 downto 0);
				DIN 		: in  	STD_LOGIC_VECTOR (31 downto 0);
				DWEN		: in 		std_logic;
				DOUT 		: out  	STD_LOGIC_VECTOR (31 downto 0);
				DDESTWE	: out 	std_logic;
				BUSY 		: out  	STD_LOGIC);
end sm_dummy_data;

architecture Behavioral of sm_dummy_data is
COMPONENT dumm_fifo
  PORT (
    clk 			: 		IN STD_LOGIC;
    srst 		: 		IN STD_LOGIC;
    din 			: 		IN STD_LOGIC_VECTOR(31 DOWNTO 0);
    wr_en 		: 		IN STD_LOGIC;
    rd_en 		: 		IN STD_LOGIC;
    dout 		: 		OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
    full 		: 		OUT STD_LOGIC;
    empty 		: 		OUT STD_LOGIC;
    prog_full 	: 		OUT STD_LOGIC;
	 data_count	:		OUT std_logic_vector(9 downto 0)
  );
END COMPONENT;

signal fifo_read_enable 					: std_logic;
signal fifo_empty 							: std_logic;
signal fifo_data_count						: std_logic_vector(9 downto 0);
signal fifo_full								: std_logic;
signal fifo_destination_write_enable 	: std_logic;
type transferstate is (idle, transfer);

signal state		      : transferstate;

begin

-- daten per din werden auf signal von data_write_enable in den fifo geschrieben. 

-- transfer von dummy daten aus dummy fifo in daten fifo 

EMPTY 			<= fifo_empty;
FULL				<= fifo_full;
DDESTWE 			<= fifo_destination_write_enable; -- and not fifo_empty;
DATA_COUNT 		<= fifo_data_count;
	
dummy_data : dumm_fifo
	port map(
		clk			=> CLK,
		srst			=> RESET,
		din			=> DIN,
		wr_en			=> DWEN,
		rd_en			=> fifo_read_enable,
		dout			=> DOUT,
		full			=> fifo_full,
		empty			=> fifo_empty,
		prog_full	=> open,
		data_count	=> fifo_data_count
	);
	
	
		DUMMY: process(CLK)
	begin
		if rising_edge(CLK) then
			if RESET = '1' then
				state         <= idle;
				BUSY          <= '0';
				fifo_read_enable <=  '0';

			else  case state is

				when idle =>
					if START = '1' then
						state <= transfer;
						fifo_read_enable <= '1';
						fifo_destination_write_enable <= '0';
						BUSY <= '1';
					else 
						BUSY <='0';
						fifo_read_enable <= '0';
						fifo_destination_write_enable <= '0';
						state <= state;
					end if;
				when transfer =>
					if fifo_empty = '0' then 
						state <= transfer; 
						fifo_read_enable <= '1';
						fifo_destination_write_enable <= '1';
						BUSY <= '1';
					else 
						state <= idle;
						fifo_read_enable <= '0';
						fifo_destination_write_enable <= '0';
						BUSY <= '0';
					end if;
				end case;
			end if;
		end if;
	end process;
end Behavioral;

