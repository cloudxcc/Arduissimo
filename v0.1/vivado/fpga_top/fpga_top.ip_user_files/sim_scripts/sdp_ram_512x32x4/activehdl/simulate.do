onbreak {quit -force}
onerror {quit -force}

asim -t 1ps +access +r +m+sdp_ram_512x32x4 -L xpm -L blk_mem_gen_v8_4_4 -L xil_defaultlib -L unisims_ver -L unimacro_ver -L secureip -O5 xil_defaultlib.sdp_ram_512x32x4 xil_defaultlib.glbl

do {wave.do}

view wave
view structure

do {sdp_ram_512x32x4.udo}

run -all

endsim

quit -force
