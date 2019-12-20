
.PHONY: downloadHex 

colors: 
	rm *.elf *.hex ;\
	cp ../../../demo/colors/ws_colors/colors_0/Release/colors_0.elf . ;\
	cp ../../../demo/colors/ws_colors/colors_1/Release/colors_1.elf . ;\
	cp ../../../demo/colors/ws_colors/colors_2/Release/colors_2.elf . ;\
	cp ../../../demo/colors/ws_colors/colors_3/Release/colors_3.elf . ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_1.elf main_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_2.elf main_2.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_3.elf main_3.hex ;\
	rm *.elf

compile: 
	cp ../source/* . ;\
	rm *.o *.exe ;\
	gcc -c downloadHex.c -I. -o downloadHex.o ;\
	gcc -o downloadHex.exe downloadHex.o -L. -lftd2xx ;\
	rm *.o *.c *.lib *.h

downloadHex :
	./downloadHex.exe -srb f -sre 0 -lb -dc 0 main_0.hex -dc 1 main_1.hex -dc 2 main_2.hex -dc 3 main_3.hex
