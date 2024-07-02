..\OpenOCD-20190426-0.10.0\bin\openocd.exe -f ..\OpenOCD-20190426-0.10.0\tcl\target\stm32f4x_cmsisdap.cfg -c "init;halt;reset halt;flash write_image erase ./build/mate_control/mate_control.bin 0x08000000;reset run;shutdown;"
pause

