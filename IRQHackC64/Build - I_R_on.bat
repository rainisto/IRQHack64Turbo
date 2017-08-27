Call PreBuild.BAT

cd Menus\I_R_on
64tass -c -b IrqLoaderMenuNew.65s -o ..\..\build\IrqLoaderMenuNew.65s.bin --labels ..\..\build\IrqLoaderMenuNew.txt
cd ..
cd ..
petcat -w2 <Menus\I_R_on\IrqLoaderMenu.bas >build\IrqLoaderMenu.obj
copy /b build\IrqLoaderMenu.obj + build\IrqLoaderMenuNew.65s.bin build\irqhack64.prg

64tass -c -b Menus\KeyBooter\KeyBooter.65s -o build\KeyBooter.65s.bin --labels build\KeyBooter.txt
copy /b build\IrqLoaderMenu.obj + build\KeyBooter.65s.bin build\keybooter.prg


Call PostBuild.BAT

PAUSE