----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:30:59 02/21/2012 
-- Design Name: 
-- Module Name:    knight_rider_timer - Behavioral 
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
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity knight_rider_timer is
    Port ( CLK     : in   STD_LOGIC;
			  STEPS   : in   integer;  -- after which CLK cycles should we give the trigger?
           TRIGGER : out  STD_LOGIC);
end knight_rider_timer;

architecture Behavioral of knight_rider_timer is
	constant clk_steps : integer := 66000000;
	signal clk_counter : integer range 0 to clk_steps-1;

begin
	process (CLK)
	begin
		if rising_edge(CLK) then
			if clk_counter = clk_steps then
				-- somehow we have reached the maximum of the clk_counter-range
				clk_counter <= 0;
			elsif clk_counter = STEPS then
				-- we have reached the end of cycle, propagate
				clk_counter <= 0;
				TRIGGER <= '1';
			else
				-- just increase the counter
				clk_counter <= clk_counter + 1;
				TRIGGER <= '0';
			end if;
		end if;
	end process;
end Behavioral;

