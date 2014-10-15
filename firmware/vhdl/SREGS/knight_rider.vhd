----------------------------------------------------------------------------------
-- Company:  IKP-1, FZ Jlich
-- Engineer: Andr Goerres
--
-- Create Date:    10:05:34 02/21/2012
-- Design Name:
-- Module Name:    knight_rider - Behavioral
-- Project Name:
-- Target Devices:
-- Tool versions:
-- Description: 	With this module you can display a running light (like the
--						Knight Rider thing) on the ML605-User-LEDs. It is controllable
--						by the User-Switches.
--						Usage:
--							switch 1:	 running on/off
--							switch 2-3:	 speed
--							switch 4-5:  brightness of second LED
--							switch 6-8:  -
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
use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity knight_rider is
	port ( CLK66   		: in	STD_LOGIC;
			 USER_SWITCH	: in	STD_LOGIC_VECTOR (4 downto 0);
			 LED     		: out	STD_LOGIC_VECTOR (7 downto 0) );
end knight_rider;



architecture Behavioral of knight_rider is
	constant clk_steps_for_1ms			: integer := 125000;  -- clock steps in one millisecond
	signal clk_steps_propagate			: integer range 0 to 125000000 := clk_steps_for_1ms * 1000;

	constant led2_on_cycles				: integer := 5;                -- how many cycles should the LED stay bright?
	signal led2_on_cycles_counter		: integer range 0 to led2_on_cycles := 0;
	signal led2_off_cycles 				: integer range 0 to 30 := 1;  -- how many cycles should the LED stay dark?
	signal led2_off_cycles_counter	: integer range 0 to 30 := 0;

	signal propagate			: STD_LOGIC := '0';  -- propagate the LED to next step if 1
	signal flip_2led			: STD_LOGIC := '0';  -- trigger to flip the second LED (for afterglow)
	signal led_status			: STD_LOGIC_VECTOR (7 downto 0) := "00000000";

	signal led_direction		: bit;  -- 0: left, 1: right
	signal led_position		: integer range 0 to 7 := 1;   -- which LED is the brightest one?
	signal led2_position		: integer range 0 to 7 := 0;  -- which is the second LED (afterglow)
	--signal led2_status		: STD_LOGIC := '0';

	component knight_rider_timer
		port ( CLK		: in   STD_LOGIC;
				 STEPS	: in   integer;
				 TRIGGER	: out  STD_LOGIC );
	end component;

begin

	-- get the trigger for LED propagation
	U_propagate: knight_rider_timer port map (
		CLK      => CLK66,
		STEPS    => clk_steps_propagate,
		TRIGGER  => propagate
	);

	-- get the trigger for flipping the second LED
	U_second_LED: knight_rider_timer port map (
		CLK      => CLK66,
		STEPS    => clk_steps_for_1ms,
		TRIGGER  => flip_2led
	);


	process (USER_SWITCH)
	begin
		-- the speed of the running light
		case USER_SWITCH(2 downto 1) is
			when "01" 	=> clk_steps_propagate <= clk_steps_for_1ms * 100;
			when "10" 	=> clk_steps_propagate <= clk_steps_for_1ms * 50;
			when "11" 	=> clk_steps_propagate <= clk_steps_for_1ms * 30;
			when others	=> clk_steps_propagate <= clk_steps_for_1ms * 1000;
		end case;

		-- the brightness of the second LED
		case USER_SWITCH(4 downto 3) is
			-- for comparison: on_cycles are 5
			when "01"	=> led2_off_cycles <= 4;
			when "10"	=> led2_off_cycles <= 8;
			when "11"	=> led2_off_cycles <= 20;
			when others	=> led2_off_cycles <= 1;
		end case;
	end process;


	-----------------------------------------------------------

	-- propagate the LED-display
	process (propagate, USER_SWITCH)
		variable tmp_direction : bit;
		--variable next_position : integer range 0 to 7;
	begin
		if rising_edge(propagate) and rising_edge(USER_SWITCH(0)) then
			-- which direction are we going?
			if led_position = 0 then
				tmp_direction := '0';
			elsif led_position = 7 then
				tmp_direction := '1';
			else
				tmp_direction := led_direction;
			end if;
			led_direction <= tmp_direction;

			-- next LED to be the brightest
			if tmp_direction = '1' then
				led_position <= led_position - 1;
			else
				led_position <= led_position + 1;
			end if;
		end if;
	end process;



	-----------------------------------------------------------

	-- produce an afterglow effect

	process (flip_2led)
		variable pos : integer range 0 to 7;
	begin
		if rising_edge(flip_2led) then
			if led_direction = '1' then
				pos := led_position + 1;
			else
				pos := led_position - 1;
			end if;

			led2_position <= pos;
			--led2_status <= led_status(pos);
		end if;
	end process;



	-----------------------------------------------------------

	-- refresh the current LED setting
	process (CLK66, led_position, led2_position, led2_off_cycles_counter)
	begin
		if rising_edge(CLK66) then
			for i in 0 to 7 loop
				if i = led_position then
					led_status(i) <= '1';

				-- the second led
				elsif i = led2_position then
					-- it is now off, should we turn it on?
					if led_status(i) = '0' then
						if led2_off_cycles_counter = led2_off_cycles then
							led2_off_cycles_counter <= 0;
							led_status(i) <= '1';
						else
							led2_off_cycles_counter <= led2_off_cycles_counter + 1;
							led_status(i) <= '0';
						end if;

					-- it is now on, should we turn it off?
					else
						if led2_on_cycles_counter = led2_on_cycles then
							led2_on_cycles_counter <= 0;
							led_status(i) <= '0';
						else
							led2_on_cycles_counter <= led2_on_cycles_counter + 1;
							led_status(i) <= '1';
						end if;
					end if;
				else
					led_status(i) <= '0';
				end if;
			end loop;
		end if;
	end process;

	LED <= led_status;

end Behavioral;

