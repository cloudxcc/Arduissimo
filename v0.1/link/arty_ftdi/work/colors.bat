del *.elf
copy ..\..\..\demo\colors\ws_colors\colors_0\Release\colors_0.elf .
copy ..\..\..\demo\colors\ws_colors\colors_1\Release\colors_1.elf .
copy ..\..\..\demo\colors\ws_colors\colors_2\Release\colors_2.elf .
copy ..\..\..\demo\colors\ws_colors\colors_3\Release\colors_3.elf .

del *.disasm *.hex
riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_0.elf main_0.hex
riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_1.elf main_1.hex
riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_2.elf main_2.hex
riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_3.elf main_3.hex

#del *.o *.exe
#copy ..\source\* .
#gcc -c downloadHex.c -I. -o downloadHex.o
#gcc -o downloadHex.exe downloadHex.o -L. -lftd2xx  

#downloadHex -srb f -sre 0 -lb -dc 0 main_0.hex -dc 1 main_1.hex -dc 2 main_2.hex -dc 3 main_3.hex
downloadHex
