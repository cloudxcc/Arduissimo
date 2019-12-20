onbreak {quit -f}
onerror {quit -f}

vsim -voptargs="+acc" -t 1ps -L xpm -L blk_mem_gen_v8_4_4 -L xil_defaultlib -L unisims_ver -L unimacro_ver -L secureip -lib xil_defaultlib xil_defaultlib.sdp_ram_512x32x4 xil_defaultlib.glbl

do {wave.do}

view wave
view structure
view signals

do {sdp_ram_512x32x4.udo}

run -all

quit -force
