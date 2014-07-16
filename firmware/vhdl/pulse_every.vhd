-- -----------------------------------------------------------------------------
--
--                         Andre Goerres  |       ######
--         Insitut fuer Kernphysik (IKP)  |    #########
--        Forschungszentrum Juelich GmbH  |   #########   ##
--              D-52425 Juelich, Germany  |  #########   ####
--                                        |  ########   #####
--            (+49) 2461 61 6225 :   Tel  |   #   ##   #####
--            (+49) 2461 61 3930 :   Fax  |    ##     #####
--       a.goerres@fz-juelich.de : E-Mail |       ######
--
-- -----------------------------------------------------------------------------
-- =============================================================================
--
--	project:		PANDA
--	module:		pulse every
--	author:		A. Goerres IKP.FZJ
--
-- History
-- Date     | Rev | Author    | What
-- ---------+-----+-----------+-------------------------------------------------
-- 15.08.12 | 1.0 | A.Goerres | First final version

-- ======================================================================= --

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;


entity pulse_every is
	generic (
		CLOCK_CYCLES_PER_PULSE	: integer
	);
	port (
		CLK 		: in  std_logic;
		RST 		: in  std_logic;
		PULSE 	: out std_logic
	);
end pulse_every;



architecture Behavioral of pulse_every is

	constant counter_max		: integer := CLOCK_CYCLES_PER_PULSE;
	signal counter				: integer range 0 to counter_max := 0;
	signal pulse_out			: std_logic := '0';
	
begin

	PULSE <= pulse_out;

	clock_divider : process (CLK, RST)
	begin 
		if rising_edge(CLK) then 
			if ( RST = '1' ) then
				counter <= 0;
				pulse_out <= '0';
			elsif ( counter = counter_max ) then 
				counter <= 0;
				pulse_out <= '1';
			else
				counter <= counter + 1;
				pulse_out <= '0';
			end if;
		end if;
	end process;

end Behavioral;

