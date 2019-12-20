onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib tdp_ram_512x32_opt

do {wave.do}

view wave
view structure
view signals

do {tdp_ram_512x32.udo}

run -all

quit -force
