-- ======================================================================= --
--
--	project:		all
--	author:		W.Erven ZEL.FZJ
--
-- Historie
-- Datum    | Rev | Autor     | Was
------------+-----+-----------+------------------------------------------- --
-- 08.09.04 | 1.0 | W.Erven   | 1. Version
--
-- ======================================================================= --
--
--	file:		utilpack.vhd
--
-- package:	util_pack		general VHDL utility functions
--
-- ======================================================================= --
--
library IEEE;
use IEEE.STD_LOGIC_1164.all;
use IEEE.NUMERIC_STD.all;

package util_pack is

	constant I80000000	: integer	:=integer'low;

	subtype T_DS		is std_logic_vector(1 downto 0);
	subtype T_SLV2		is std_logic_vector(1 downto 0);
	subtype T_SLV3		is std_logic_vector(2 downto 0);
	subtype T_SLV4		is std_logic_vector(3 downto 0);
	subtype T_SLV5		is std_logic_vector(4 downto 0);
	subtype T_SLV6		is std_logic_vector(5 downto 0);
	subtype T_SLV8		is std_logic_vector(7 downto 0);
	subtype T_SLV16	is std_logic_vector(15 downto 0);
	subtype T_SLV24	is std_logic_vector(23 downto 0);
	subtype T_SLV28	is std_logic_vector(27 downto 0);
	subtype T_SLV32	is std_logic_vector(31 downto 0);

	type T_8DS			is array(7 downto 0) of T_DS;
	type T_16DS			is array(15 downto 0) of T_DS;

	type T_8SLV8		is array(7 downto 0) of T_SLV8;
	type T_8SLV16		is array(7 downto 0) of T_SLV16;
	type T_8SLV32		is array(7 downto 0) of T_SLV32;

	---------------------------------------------------------------------------
	-- Function INT2CHAR converts integer to character
	---------------------------------------------------------------------------
	function INT2CHAR(i: integer) return character;

	---------------------------------------------------------------------------
	-- Function B2SL converts boolean to standard logic
	---------------------------------------------------------------------------
	function B2SL(bool: boolean) return std_logic;

	---------------------------------------------------------------------------
	-- Function SL2B converts standard logic to boolean
	---------------------------------------------------------------------------
	function SL2B(sl: std_logic) return boolean;

	---------------------------------------------------------------------------
	-- Function SFLOAT2ZERO converts 'Z' signals to '0' signals
	---------------------------------------------------------------------------
	function SFLOAT2ZERO(SL: std_logic) return std_logic;

	---------------------------------------------------------------------------
	-- Function FLOAT2ZERO converts 'Z' signals to '0' signals
	---------------------------------------------------------------------------
	function FLOAT2ZERO(SLV: std_logic_vector) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function EXT2SLV return SLV extended to n
	---------------------------------------------------------------------------

	function EXT2SLV(
		SLV:	std_logic_vector;
		n:		integer :=32
	) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function EXT2SLV16 return standard logic extended to 16
	---------------------------------------------------------------------------

	function EXT2SLV16(SLV: std_logic_vector) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function SEXT2SLV return signed SLV extended to n
	---------------------------------------------------------------------------

	function SEXT2SLV(
		SLV:	std_logic_vector;
		n:		integer :=32
	) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function SL2SLV return standard logic multiplied SLV to n
	---------------------------------------------------------------------------

	function SL2SLV(
		SL:	std_logic;
		n:		integer :=32
	) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function SL2SLV16 return standard logic multiplied SLV to 16
	---------------------------------------------------------------------------

	function SL2SLV16(SL: std_logic) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function Z2SLV return zero std_logic_vector
	---------------------------------------------------------------------------
	function Z2SLV(size: integer) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function HEX2SLV converts a hexadecimal string to std_logic_vector
	-- of size size-1 downto 0.
	---------------------------------------------------------------------------
	function HEX2SLV(
		str : string;
		size: integer :=32
	) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function INT2SLV converts an integer to a std_logic_vector
	---------------------------------------------------------------------------
	function INT2SLV(
		val : integer;
		size: integer :=32
	) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function INT2SLV16 converts an integer to a 16 bit std_logic_vector
	---------------------------------------------------------------------------
	function INT2SLV16 (
		val : integer
	) return std_logic_vector;

	---------------------------------------------------------------------------
	-- Function SL2INT converts standard logic into an integer
	---------------------------------------------------------------------------
	function SL2INT(SL: std_logic) return integer;

	---------------------------------------------------------------------------
	-- Function SLV2INT converts standard logic vector into an integer
	-- Xilinx\vhdl\src\unisims\unisim_VPKG.vhd
	---------------------------------------------------------------------------
	function SLV2INT(SLV: std_logic_vector) return integer;

	---------------------------------------------------------------------------
	-- Function SLV2NAT converts standard logic vector into an natural subtype
	---------------------------------------------------------------------------
	function SLV2NAT(SLV: std_logic_vector) return natural;

	---------------------------------------------------------------------------
	-- Function INT2SLV32 converts an integer to a 32 bit std_logic_vector
	---------------------------------------------------------------------------
	function INT2SLV32 (
		val : integer
	) return std_logic_vector;

end util_pack;

-- ======================================================================= --

package body util_pack is

	---------------------------------------------------------------------------
	-- Function INT2CHAR converts integer to character
	---------------------------------------------------------------------------
	function INT2CHAR(i: integer
	) return character is

	begin
	case i is
		when 0 => return '0';
		when 1 => return '1';
		when 2 => return '2';
		when 3 => return '3';
		when 4 => return '4';
		when 5 => return '5';
		when 6 => return '6';
		when 7 => return '7';
		when 8 => return '8';
		when 9 => return '9';
		when others => return ' ';
	end case;
	end;

	---------------------------------------------------------------------------
	-- Function B2SL converts boolean to standard logic
	---------------------------------------------------------------------------

	function B2SL(bool: boolean
	) return std_logic is

	begin
	if (bool) then
		return '1';
	else
		return '0';
	end if;
	end;

	---------------------------------------------------------------------------
	-- Function SL2B converts standard logic to boolean
	---------------------------------------------------------------------------

	function SL2B(sl: std_logic
	) return boolean is

	begin
	if (sl = '1') then
		return true;
	else
		return false;
	end if;
	end;

	---------------------------------------------------------------------------
	-- Function SFLOAT2ZERO converts 'Z' signals to '0' signals
	---------------------------------------------------------------------------
	function SFLOAT2ZERO(SL: std_logic
	) return std_logic is

	begin
	return SL;
	end;





	---------------------------------------------------------------------------
	-- Function FLOAT2ZERO converts 'Z' signals to '0' signals
	---------------------------------------------------------------------------
	function FLOAT2ZERO(SLV: std_logic_vector
	) return std_logic_vector is

	begin
	return SLV;
	end;










	---------------------------------------------------------------------------
	-- Function EXT2SLV return standard logic vector extended to n
	---------------------------------------------------------------------------

	function EXT2SLV(
		SLV:	std_logic_vector;
		n:		integer :=32
	) return std_logic_vector is

	variable result: std_logic_vector(n-1 downto 0);

	begin
		if (SLV'length = n) then
			result := SLV;
		else
			result := Z2SLV(n-SLV'length)&SLV;
		end if;

		return result;
	end;

	---------------------------------------------------------------------------
	-- Function EXT2SLV16 return standard logic vector extended to 16
	---------------------------------------------------------------------------

	function EXT2SLV16(SLV: std_logic_vector
	) return std_logic_vector is

	begin
	return EXT2SLV(SLV, 16);
	end;

	---------------------------------------------------------------------------
	-- Function SEXT2SLV return signed standard logic vector extended to n
	---------------------------------------------------------------------------

	function SEXT2SLV(
		SLV:	std_logic_vector;
		n:		integer :=32
	) return std_logic_vector is

	variable result: std_logic_vector(n-1 downto 0);

	begin
		if (SLV'length = n) then
			result := SLV;
		else
			result := SL2SLV(SLV(SLV'high), n-SLV'length)&SLV;
		end if;

		return result;
	end;

	---------------------------------------------------------------------------
	-- Function SL2SLV return standard logic multiplied SLV to n
	---------------------------------------------------------------------------

	function SL2SLV(
		SL:	std_logic;
		n:		integer :=32
	) return std_logic_vector is

	variable result: std_logic_vector(n-1 downto 0);

	begin
--		result := (others => SL);  -- this creates warnings in the synthesizer
		for i in 0 to (n-1) loop
			result(i) := SL;
		end loop;
		return result;
	end;

	---------------------------------------------------------------------------
	-- Function SL2SLV16 return standard logic multiplied SLV to 16
	---------------------------------------------------------------------------

	function SL2SLV16(SL: std_logic
	) return std_logic_vector is

	begin
	return SL2SLV(SL, 16);
	end;

	---------------------------------------------------------------------------
	-- Function Z2SLV return zero std_logic_vector
	---------------------------------------------------------------------------

	function Z2SLV(size: integer
	) return std_logic_vector is

	variable result: std_logic_vector(size-1 downto 0);
	begin
	result := (others => '0');
	return result;
	end;

	---------------------------------------------------------------------------
	-- Function HEX2SLV converts a hexadecimal string to std_logic_vector
	-- of size size-1 downto 0.
	---------------------------------------------------------------------------
	function HEX2SLV(
		str : string;
		size: integer :=32
	) return std_logic_vector is

	variable slv	: std_logic_vector(15 downto 0) := (others => '0');
	variable j		: integer :=0;

	begin
	  for I in str'high to str'low loop
		 case str(I) is
			when '0' =>
			  slv(j*4+3 downto j*4) := "0000";
			when '1' =>
			  slv(j*4+3 downto j*4) := "0001";
			when '2' =>
			  slv(j*4+3 downto j*4) := "0010";
			when '3' =>
			  slv(j*4+3 downto j*4) := "0011";
			when '4' =>
			  slv(j*4+3 downto j*4) := "0100";
			when '5' =>
			  slv(j*4+3 downto j*4) := "0101";
			when '6' =>
			  slv(j*4+3 downto j*4) := "0110";
			when '7' =>
			  slv(j*4+3 downto j*4) := "0111";
			when '8' =>
			  slv(j*4+3 downto j*4) := "1000";
			when '9' =>
			  slv(j*4+3 downto j*4) := "1001";
			when 'a' | 'A' =>
			  slv(j*4+3 downto j*4) := "1010";
			when 'b' | 'B' =>
			  slv(j*4+3 downto j*4) := "1011";
			when 'c' | 'C' =>
			  slv(j*4+3 downto j*4) := "1100";
			when 'd' | 'D' =>
			  slv(j*4+3 downto j*4) := "1101";
			when 'e' | 'E' =>
			  slv(j*4+3 downto j*4) := "1110";
			when 'f' | 'F' =>
			  slv(j*4+3 downto j*4) := "1111";
			when others =>
			  assert false
				 report "WARNING: Unknown Hex digit in str: "&str(I)
				 severity failure;
			  slv(j*4+3 downto j*4)   := "XXXX";
		 end case;

		 j	:= j+1;
	  end loop;

	  return slv;

	end HEX2SLV;

	---------------------------------------------------------------------------
	-- Function INT2SLV converts an integer to a std_logic_vector
	---------------------------------------------------------------------------
	function INT2SLV(
		val : integer;
		size: integer :=32
	) return std_logic_vector is

		variable result: std_logic_vector(size-1 downto 0);
		variable temp: integer;

	begin
		temp := val;
		for i in 0 to size-1 loop
			if (temp mod 2) = 1 then
				result(i) := '1';
			else
				result(i) := '0';
			end if;

			if temp > 0 then
				temp := temp / 2;
			elsif (temp > integer'low) then
				temp := (temp - 1) / 2; -- simulate ASR
			else
				temp := temp / 2; -- simulate ASR
			end if;
		end loop;

		return result;
	end INT2SLV;

	---------------------------------------------------------------------------
	-- Function INT2SLV16 converts an integer to a 16 bit std_logic_vector
	---------------------------------------------------------------------------
	function INT2SLV16(
		val : integer
	) return std_logic_vector is

	begin
		return INT2SLV(val, 16);

	end INT2SLV16;

	---------------------------------------------------------------------------
	-- Function SL2INT converts standard logic into an integer
	---------------------------------------------------------------------------

	function SL2INT(SL: std_logic
	) return integer is

	begin
	if (SL = '0') then
		return 0;
	else
		return 1;
	end if;
	end;

	---------------------------------------------------------------------------
	-- Function SLV2INT converts standard logic vector into an integer
	-- Xilinx\vhdl\src\unisims\unisim_VPKG.vhd
	---------------------------------------------------------------------------

	function SLV2INT(SLV: std_logic_vector
	) return integer is

	variable int : integer;
	begin
		int := 0;
		for i in SLV'high downto SLV'low loop
			 int := int * 2;
			 if SLV(i) = '1' then
				  int := int + 1;
			 end if;
		end loop;
		return int;
	end;

	---------------------------------------------------------------------------
	-- Function SLV2NAT converts standard logic vector into an unsigned
	-- Xilinx\vhdl\src\unisims\unisim_VPKG.vhd
	---------------------------------------------------------------------------
function SLV2NAT(SLV: std_logic_vector
	) return natural is

	subtype res_type is natural range 0 to 2**SLV'length-1;
	variable result : res_type;
	begin
		result := 0;
		for i in SLV'high downto SLV'low loop
			 result := result +result;
			 if SLV(i) = '1' then
				  result := result +1;
			 end if;
		end loop;
		return result;
	end;

	---------------------------------------------------------------------------
	-- Function INT2SLV32 converts an integer to a 32 bit std_logic_vector
	---------------------------------------------------------------------------
	function INT2SLV32(
		val : integer
	) return std_logic_vector is

	begin
		return INT2SLV(val);

	end INT2SLV32;

-- ======================================================================== --

end util_pack;
