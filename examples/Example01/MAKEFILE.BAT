sdcc -mz80 --code-loc 0x4020 --data-loc 0xC000 --use-stdout --no-std-crt0 crt0_MSX816kROM4000.rel VDP_TMS9918A_MSXBIOS.rel Example01.c
hex2bin -e bin -l 2000 Example01.ihx
copy Example01.bin EXAMPLE1.ROM
pause
