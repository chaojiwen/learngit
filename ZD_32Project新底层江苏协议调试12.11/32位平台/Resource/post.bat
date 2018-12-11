del ..\Output\Firmware.bin
fsutil  file createnew ..\Output\Firmware.bin 12288
..\Resource\cat "..\Output\App.bin" >>"..\Output\Firmware.bin"
..\Resource\hbin "..\Resource\Bootloader.bin" "..\Output\Firmware.bin"
..\Resource\bin2HEX  /4  /O0x08000000 "..\Output\Firmware.bin" "..\Output\Firmware.hex"