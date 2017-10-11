FROM debian:stable

RUN apt-get update

RUN DEBIAN_FRONTEND=noninteractive apt-get -y install git curl nano vim make gcc unzip
RUN curl -L https://sourceforge.net/projects/tass64/files/source/64tass-1.53.1515-src.zip/download -o 64tass-1.53.1515-src.zip && \
    unzip 64tass-1.53.1515-src.zip && cd 64tass-1.53.1515-src && make && make install && cd - && \
    curl -L ftp://ftp.zimmers.net/pub/cbm/programming/unix/petcat-1.9.tar.gz -o petcat-1.9.tar.gz && tar -zxvf petcat-1.9.tar.gz && \
    cd petcat-1.9 && make -f Makefile.petcat && cp petcat /bin && cd - && \
    git clone https://github.com/meonwax/acme.git && \
    cd acme/src && make && make install
RUN git clone https://github.com/rainisto/IRQHack64Turbo.git && cd /IRQHack64Turbo/IRQHackC64 && \
    64tass -c -b Loader/LoaderStub.65s -o build/LoaderStub.65s.bin --labels build/LoaderStub.65s.txt && \
    64tass -c -b Loader/IRQLoader.65s -o build/IRQLoader.65s.bin --labels build/IRQLoader.txt && \
    petcat -w2 < Menus/I_R_on/IrqLoaderMenu.bas > build/IrqLoaderMenu.obj && \
    64tass -c -b Menus/WarningMenu/Warning.65s -o build/Warning.65s.bin  --labels build/Warning.65s.txt && \
    cat build/IrqLoaderMenu.obj build/Warning.65s.bin > build/warning.prg && \
    cd Menus/Wizofwor && acme -l labels.txt -r Report.txt main.asm && cd - && \
    cp Menus/Wizofwor/build/menu.prg build/irqhack64.prg
RUN cd /IRQHack64Turbo/IRQHackC64/Menus/I_R_on && 64tass -c -b IrqLoaderMenuNew.65s -o ../../build/IrqLoaderMenuNew.65s.bin --labels ../../build/IrqLoaderMenuNew.txt && cd ../.. && \
    petcat -w2 < Menus/I_R_on/IrqLoaderMenu.bas > build/IrqLoaderMenu.obj && cat build/IrqLoaderMenu.obj build/IrqLoaderMenuNew.65s.bin > build/irqhack64IRon.prg && \
    64tass -c -b Menus/Keybooter/KeyBooter.65s -o build/KeyBooter.65s.bin --labels build/KeyBooter.txt && \
    cat build/IrqLoaderMenu.obj build/KeyBooter.65s.bin > build/keybooter.prg

CMD (cd /IRQHack64Turbo/IRQHackC64/build && cp * /flash/)
