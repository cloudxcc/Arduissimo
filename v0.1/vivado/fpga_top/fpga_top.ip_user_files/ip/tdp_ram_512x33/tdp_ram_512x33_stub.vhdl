-- Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
-- --------------------------------------------------------------------------------
-- Tool Version: Vivado v.2019.2 (win64) Build 2708876 Wed Nov  6 21:40:23 MST 2019
-- Date        : Sat Nov 30 20:20:05 2019
-- Host        : asuspc running 64-bit major release  (build 9200)
-- Command     : write_vhdl -force -mode synth_stub -rename_top tdp_ram_512x33 -prefix
--               tdp_ram_512x33_ tdp_ram_512x33_stub.vhdl
-- Design      : tdp_ram_512x33
-- Purpose     : Stub declaration of top-level module interface
-- Device      : xc7a35ticsg324-1L
-- --------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

entity tdp_ram_512x33 is
  Port ( 
    clka : in STD_LOGIC;
    wea : in STD_LOGIC_VECTOR ( 0 to 0 );
    addra : in STD_LOGIC_VECTOR ( 8 downto 0 );
    dina : in STD_LOGIC_VECTOR ( 32 downto 0 );
    douta : out STD_LOGIC_VECTOR ( 32 downto 0 );
    clkb : in STD_LOGIC;
    web : in STD_LOGIC_VECTOR ( 0 to 0 );
    addrb : in STD_LOGIC_VECTOR ( 8 downto 0 );
    dinb : in STD_LOGIC_VECTOR ( 32 downto 0 );
    doutb : out STD_LOGIC_VECTOR ( 32 downto 0 )
  );

end tdp_ram_512x33;

architecture stub of tdp_ram_512x33 is
attribute syn_black_box : boolean;
attribute black_box_pad_pin : string;
attribute syn_black_box of stub : architecture is true;
attribute black_box_pad_pin of stub : architecture is "clka,wea[0:0],addra[8:0],dina[32:0],douta[32:0],clkb,web[0:0],addrb[8:0],dinb[32:0],doutb[32:0]";
attribute x_core_info : string;
attribute x_core_info of stub : architecture is "blk_mem_gen_v8_4_4,Vivado 2019.2";
begin
end;
