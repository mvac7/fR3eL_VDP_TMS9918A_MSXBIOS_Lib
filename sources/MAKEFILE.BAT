@echo off
REM SDCC OBJ MAKEFILE
SET CFILENAME=VDP_TMS9918A_MSXBIOS
if exist build\ goto NEXTSTEP1
echo MAKEDIR build
md build
:NEXTSTEP1
sdcc -v
echo Compiling...
sdcc -mz80 -c -o build\ --use-stdout src\%CFILENAME%.c
pause
exit /b
