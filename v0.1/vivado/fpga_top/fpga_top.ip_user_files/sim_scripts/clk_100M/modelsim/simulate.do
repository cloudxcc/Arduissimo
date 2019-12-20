onbreak {quit -f}
onerror {quit -f}

vsim -voptargs="+acc" -t 1ps -L xpm -L xil_defaultlib -L unisims_ver -L unimacro_ver -L secureip -lib xil_defaultlib xil_defaultlib.clk_100M xil_defaultlib.glbl

do {wave.do}

view wave
view structure
view signals

do {clk_100M.udo}

run -all

quit -force
