onbreak {quit -f}
onerror {quit -f}

vsim -t 1ps -lib xil_defaultlib sdp_ram_512x32x4_opt

do {wave.do}

view wave
view structure
view signals

do {sdp_ram_512x32x4.udo}

run -all

quit -force
