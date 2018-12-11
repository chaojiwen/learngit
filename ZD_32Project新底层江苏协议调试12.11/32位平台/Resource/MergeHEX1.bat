fsutil  file createnew ..\Output\Firmware.bin 12288
D:\Keil_v5\ARM\BIN40\fromelf.exe --bin -o "..\Output\App.bin" "..\Output\32_BIT.axf"
..\Resource\cat "..\Output\App.bin" >>"..\Output\Firmware.bin"
..\Resource\hbin "..\Resource\Bootloader.bin" "..\Output\Firmware.bin"
..\Resource\BIN2HEX  /4  /O0x08000000 "..\Output\Firmware.bin" "..\Output\Firmware.hex"
del "..\Output\Firmware.bin"


