----------------------------------------------------------------------------------
-- Company:
-- Engineer:
--
-- Create Date:    14:22:34 02/07/2011
-- Design Name:
-- Module Name:    counter - Behavioral
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
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity one_hertz_counter is
    Port ( CLK : in  STD_LOGIC;
         COUNTER_OUT : out  std_logic_vector (7 downto 0));

end one_hertz_counter;

architecture Behavioral of one_hertz_counter is
constant counter_div : integer := 125000000;
signal cnt : integer range 0 to counter_div -1;
signal ce : std_logic := '0';
signal counter : std_logic_vector (7 downto 0):="00000000";
begin

process(ce)
begin

if rising_edge(ce) then

counter <= counter +1;

end if;
end process ;

process(CLK)
begin
if rising_edge(CLK) then
if cnt=counter_div-1 then
	ce<='1';
	cnt <= 0;
	else
	ce <= '0';
	cnt <= cnt +1 ;
	end if;
end if;
end process;

COUNTER_OUT <= counter;

end Behavioral;


