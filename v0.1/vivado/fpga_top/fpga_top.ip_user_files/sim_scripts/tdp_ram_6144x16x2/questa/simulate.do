onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib tdp_ram_6144x16x2_opt

do {wave.do}

view wave
view structure
view signals

do {tdp_ram_6144x16x2.udo}

run -all

quit -force
