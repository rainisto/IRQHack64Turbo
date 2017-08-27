64tass -c -b Menus\WarningMenu\Warning.65s -o build\Warning.65s.bin  --labels build\Warning.65s.txt
copy /b build\IrqLoaderMenu.obj + build\Warning.65s.bin build\warning.prg

..\tools\bin2Ardh build\warning.prg build\defaultmenu.h data_len cartridgeData
..\tools\bin2Ardh build\LoaderStub.65s.bin build\LoaderStub.h stub_len stubData
copy avrincludehead.txt+build\defaultmenu.h build\head.tmp
copy build\head.tmp + build\LoaderStub.h build\final.tmp

copy build\final.tmp+avrincludefoot.txt  build\FlashLib.h
copy build\FlashLib.h ..\Arduino\IRQHack64\FlashLib.h



del build\defaultmenu.h
del build\IrqLoaderMenu.obj
del build\final.tmp
del build\LoaderStub.h


..\tools\CreateEpromLoader build\IRQLoader.65s.bin build\IRQLoaderRom.bin  151 146 103 161 121 171 166 141 176 156