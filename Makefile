all: arcade_sim

clean:
	rm arcade_sim arduino_stub.o
arduino_stub.o: arduino_stub.cpp $(wildcard CLUE_Arcade/*.ino) $(wildcard arduino_stubs/*.h)
	$(CXX) -Iarduino_stubs -c -o arduino_stub.o arduino_stub.cpp

LDLIBS=-lX11 -lm -lpthread -lstdc++
CCFLAGS=-pthread
arcade_sim: arduino_stub.o arcade_sim.c

