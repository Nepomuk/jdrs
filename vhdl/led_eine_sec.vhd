-- -----------------------------------------------------------------------------
--
--                           Simone Esch  |       ######
--          Insitut fuer Kernphysik(IKP)  |    #########
--        Forschungszentrum Juelich GmbH  |   #########   ##
--              D-52425 Juelich, Germany  |  #########   ####
--                                        |  ########   #####
--             (+49)2461 61 3101 :   Tel  |   #   ##   #####
--             (+49)2461 61 3930 :   FAX  |    ##     #####
--          s.esch@fz-juelich.de : EMail  |       ######
--
-- -----------------------------------------------------------------------------
-- =============================================================================
--
--  project:    PANDA
--  module:   led_eine_sec
--  author:   S.Esch IKP.FZJ
--
-- History
-- Date     | Rev | Author    | What
-- ---------+-----+-----------+-------------------------------------------------
-- 11.02.11 | 0.0 | S.Esch    | Begin of design

-- ======================================================================= --
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;


entity led_eine_sec is
  port (
    CLK       : in  std_logic;
    TRIGGER   : in  std_logic;
    LED_TEST  : out std_logic;
    LED_OUT   : out std_logic
  );
end led_eine_sec;

architecture Behavioral of led_eine_sec is

  constant counter_div: integer:=12500000;
  signal cnt : integer range 0 to counter_div -1;
  signal trigger_stop : std_logic :='0';
  signal led_intern : std_logic :='1';
  signal cnt_running : std_logic :='0';

begin

  LED_OUT <= led_intern;
  LED_TEST <= '1';

  led_intern  <= TRIGGER or cnt_running;

  process(clk)
  begin
    if rising_edge(clk) then
      if led_intern = '1' then
        if cnt=counter_div-1 then
          cnt <= 0;
          cnt_running <= '0';
        else
          cnt <= cnt +1 ;
          cnt_running <= '1';
        end if;
      end if;
    end if;
  end process;
end Behavioral;


