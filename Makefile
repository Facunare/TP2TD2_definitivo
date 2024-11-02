GCC=mingw32-gcc
GPP=mingw32-g++
CFLAGS= -fshow-column -fshow-column -fno-diagnostics-show-caret  -g2 -Wall -O0    -D_DEBUG 
CXXFLAGS= -fshow-column -fshow-column -fno-diagnostics-show-caret  -g2 -Wall -O0    -D_DEBUG -std=c++14
LDFLAGS= -static-libstdc++ -static-libgcc   
OBJS=Debug\main.o Debug\predict.o Debug\utils.o 

all: Debug Debug\TP2TD2_Arechaga_Bergman_Ahn.exe

clean:
	del ${OBJS} Debug\TP2TD2_Arechaga_Bergman_Ahn.exe

Debug\TP2TD2_Arechaga_Bergman_Ahn.exe: ${OBJS}
	${GPP} ${OBJS} ${LDFLAGS} -o $@

Debug:
	mkdir Debug

Debug\main.o: main.c utils.h
	${GCC} ${CFLAGS} -c main.c -o $@

Debug\predict.o: predict.c utils.h
	${GCC} ${CFLAGS} -c predict.c -o $@

Debug\utils.o: utils.c utils.h
	${GCC} ${CFLAGS} -c utils.c -o $@
