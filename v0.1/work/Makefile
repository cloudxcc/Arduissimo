#
# Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#

codeGenHex :
	cd ../tests/codeGenHex  ;\
	g++ codeGenHex.cpp -o codeGenHex.exe ;\
	./codeGenHex.exe ;\
	cd ../../work  
	      
demo_colors :
	cd ../demo/colors/work ;\
	cp ../ws_colors/colors_0/Release/colors_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S colors_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog colors_0.elf data_0.hex ;\
	cp ../ws_colors/colors_1/Release/colors_1.elf . ;\
	riscv64-unknown-elf-objdump.exe -S colors_1.elf > main_1.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_1.elf main_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog colors_1.elf data_1.hex ;\
	cp ../ws_colors/colors_2/Release/colors_2.elf . ;\
	riscv64-unknown-elf-objdump.exe -S colors_2.elf > main_2.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_2.elf main_2.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog colors_2.elf data_2.hex ;\
	cp ../ws_colors/colors_3/Release/colors_3.elf . ;\
	riscv64-unknown-elf-objdump.exe -S colors_3.elf > main_3.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog colors_3.elf main_3.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog colors_3.elf data_3.hex

drivers_pwm :
	cd ../tests/drivers/drivers_pwm/work ;\
	cp ../ws_drivers_pwm/drivers_pwm_0/Release/drivers_pwm_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_pwm_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_pwm_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_pwm_0.elf data_0.hex 

drivers_rs232 :
	cd ../tests/drivers/drivers_rs232/work ;\
	cp ../ws_drivers_rs232/drivers_rs232_0/Release/drivers_rs232_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_rs232_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_rs232_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_rs232_0.elf data_0.hex 

demo_drivers_rs232 :
	cd ../demo/drivers_rs232/work ;\
	cp ../ws_drivers_rs232/drivers_rs232_0/Release/drivers_rs232_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_rs232_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_rs232_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_rs232_0.elf data_0.hex 

drivers_i2c :
	cd ../tests/drivers/drivers_i2c/work ;\
	cp ../ws_drivers_i2c/drivers_i2c_0/Release/drivers_i2c_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_i2c_0.elf > drivers_i2c_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_i2c_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_i2c_0.elf data_0.hex ;\
	cp ../ws_drivers_i2c/drivers_i2c_1/Release/drivers_i2c_1.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_i2c_1.elf > drivers_i2c_1.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_i2c_1.elf main_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_i2c_1.elf data_1.hex

demo_drivers_i2c :
	cd ../demo/drivers_i2c/work ;\
	cp ../ws_drivers_i2c/drivers_i2c_0/Release/drivers_i2c_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_i2c_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_i2c_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_i2c_0.elf data_0.hex 

drivers_spi :
	cd ../tests/drivers/drivers_spi/work ;\
	cp ../ws_drivers_spi/drivers_spi_0/Release/drivers_spi_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_spi_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_spi_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_spi_0.elf data_0.hex ;\
	cp ../ws_drivers_spi/drivers_spi_1/Release/drivers_spi_1.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_spi_1.elf > main_1.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_spi_1.elf main_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_spi_1.elf data_1.hex

demo_drivers_spi :
	cd ../demo/drivers_spi/work ;\
	cp ../ws_drivers_spi/drivers_spi_0/Release/drivers_spi_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S drivers_spi_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog drivers_spi_0.elf drivers_spi_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog drivers_spi_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog drivers_spi_0.elf data_0.hex 

blinky64 :
	cd ../tests/system/blinky64/work ;\
	cp ../ws_blinky64/blinky64_0/Release/blinky64_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blinky64_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog blinky64_0.elf blinky64_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog blinky64_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog blinky64_0.elf data_0.hex ;\
	cp ../ws_blinky64/blinky64_1/Release/blinky64_1.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blinky64_1.elf > main_1.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog blinky64_1.elf blinky64_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog blinky64_1.elf main_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog blinky64_1.elf data_1.hex ;\
	cp ../ws_blinky64/blinky64_2/Release/blinky64_2.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blinky64_2.elf > main_2.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog blinky64_2.elf blinky64_2.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog blinky64_2.elf main_2.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog blinky64_2.elf data_2.hex ;\
	cp ../ws_blinky64/blinky64_3/Release/blinky64_3.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blinky64_3.elf > main_3.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog blinky64_3.elf blinky64_3.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog blinky64_3.elf main_3.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog blinky64_3.elf data_3.hex

demo_blinky64 :
	cd ../demo/blinky64/work ;\
	cp ../ws_blinky64/blinky64_0/Release/blinky64_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blinky64_0.elf > main_0.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog blinky64_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog blinky64_0.elf data_0.hex ;\
	cp ../ws_blinky64/blinky64_1/Release/blinky64_1.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blinky64_1.elf > main_1.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog blinky64_1.elf main_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog blinky64_1.elf data_1.hex ;\
	cp ../ws_blinky64/blinky64_2/Release/blinky64_2.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blinky64_2.elf > main_2.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog blinky64_2.elf main_2.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog blinky64_2.elf data_2.hex ;\
	cp ../ws_blinky64/blinky64_3/Release/blinky64_3.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blinky64_3.elf > main_3.disasm ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog blinky64_3.elf main_3.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog blinky64_3.elf data_3.hex

system_top :
	cd ../tests/system/system_top/work ;\
	cp ../ws_system_top/system_top_0/Release/system_top_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_top_0.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_top_0.elf system_top_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_top_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_top_0.elf data_0.hex

system_0 :
	cd ../tests/system/system_0/work ;\
	cp ../ws_system_0/system_0_0/Release/system_0_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_0_0.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_0_0.elf system_0_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_0_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_0_0.elf data_0.hex

system_1 :
	cd ../tests/system/system_1/work ;\
	cp ../ws_system_1/system_1_0/Release/system_1_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_1_0.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_1_0.elf system_1_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_1_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_1_0.elf data_0.hex

system_2 :
	cd ../tests/system/system_2/work ;\
	cp ../ws_system_2/system_2_1/Release/system_2_1.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_2_1.elf > main_1.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_2_1.elf system_2_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_2_1.elf main_1.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_2_1.elf data_1.hex ;\
	cp ../ws_system_2/system_2_2/Release/system_2_2.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_2_2.elf > main_2.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_2_2.elf system_2_2.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_2_2.elf main_2.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_2_2.elf data_2.hex ;\
	cp ../ws_system_2/system_2_3/Release/system_2_3.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_2_3.elf > main_3.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_2_3.elf system_2_3.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_2_3.elf main_3.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_2_3.elf data_3.hex

system_3 :
	cd ../tests/system/system_3/work ;\
	cp ../ws_system_3/system_3_0/Release/system_3_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_3_0.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_3_0.elf system_3_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_3_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_3_0.elf data_0.hex

system_4 :
	cd ../tests/system/system_4/work ;\
	cp ../ws_system_4/system_4_0/Release/system_4_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_4_0.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_4_0.elf system_4_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_4_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_4_0.elf data_0.hex

system_5 :
	cd ../tests/system/system_5/work ;\
	cp ../ws_system_5/system_5_0/Release/system_5_0.elf . ;\
	riscv64-unknown-elf-objdump.exe -S system_5_0.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog system_5_0.elf system_5_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.text -O verilog system_5_0.elf main_0.hex ;\
	riscv64-unknown-elf-objcopy.exe -j.data -O verilog system_5_0.elf data_0.hex

motion_32 :
	cd ../tests/chstone/motion/work ;\
	cp ../ws_rv32imac/motion/Release/motion.elf . ;\
	riscv64-unknown-elf-objdump.exe -S motion.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog motion.elf main.hex

gsm_32 :
	cd ../tests/chstone/gsm/work ;\
	cp ../gsm_ws_rv32imc/gsm/Release/gsm.elf . ;\
	riscv64-unknown-elf-objdump.exe -S gsm.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog gsm.elf main.hex

aes_32 :
	cd ../tests/chstone/aes/work ;\
	cp ../aes_ws_rv32imc/aes/Release/aes.elf . ;\
	riscv64-unknown-elf-objdump.exe -S aes.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog aes.elf main.hex

adpcm_32 :
	cd ../tests/chstone/adpcm/work ;\
	cp ../adpcm_ws_rv32imc/adpcm/Release/adpcm.elf . ;\
	riscv64-unknown-elf-objdump.exe -S adpcm.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog adpcm.elf main.hex

blowfish_32 :
	cd ../tests/chstone/blowfish/work ;\
	cp ../blowfish_ws_rv32imc/blowfish/Release/blowfish.elf . ;\
	riscv64-unknown-elf-objdump.exe -S blowfish.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog blowfish.elf main.hex

sha_32 :
	cd ../tests/chstone/sha/work ;\
	cp ../sha_ws_rv32imc/sha/Release/sha.elf . ;\
	riscv64-unknown-elf-objdump.exe -S sha.elf > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog sha.elf main.hex

branch1_32 :
	cd ../tests/basicC/branch/branch1 ;\
	riscv64-unknown-elf-gcc.exe -march=rv32im -mabi=ilp32 -c main.c ;\
	riscv64-unknown-elf-objdump.exe -S main.o > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog main.o main.hex

branch2_32 :
	cd ../tests/basicC/branch/branch2 ;\
	riscv64-unknown-elf-gcc.exe -march=rv32im -mabi=ilp32 -c main.c ;\
	riscv64-unknown-elf-objdump.exe -S main.o > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog main.o main.hex

branch3_32 :
	cd ../tests/basicC/branch/branch3 ;\
	riscv64-unknown-elf-gcc.exe -march=rv32im -mabi=ilp32 -c main.c ;\
	riscv64-unknown-elf-objdump.exe -S main.o > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog main.o main.hex

math1_32 :
	cd ../tests/basicC/math/math1 ;\
	riscv64-unknown-elf-gcc.exe -march=rv32im -mabi=ilp32 -c main.c ;\
	riscv64-unknown-elf-objdump.exe -S main.o > main.disasm ;\
	riscv64-unknown-elf-objcopy.exe -O verilog main.o main.hex
