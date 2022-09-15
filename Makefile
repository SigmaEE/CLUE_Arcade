all: x

arduino_stub.o: arduino_stub.cpp $(wildcard CLUE_Arcade/*.ino)
	$(CXX) -Iarduino_stubs -c arduino_stub.o arduino_stub.cpp


LDLIBS=-lX11 -lm -lpthread -lstdc++
CCFLAGS=-pthread
x: arduino_stub.o x.c

