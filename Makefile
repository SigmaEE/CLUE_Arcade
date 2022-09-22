all: arcade_sim

clean:
	rm arcade_sim arduino_stub.o
arduino_stub.o: arduino_stub.cpp $(wildcard CLUE_Arcade/*.ino) $(wildcard arduino_stubs/*.h)
	$(CXX) -Iarduino_stubs -c -o arduino_stub.o arduino_stub.cpp

ino_target: arduino_stub.o
	/home/hlofving/Downloads/arduino-1.8.19/arduino-builder -compile -logger=machine -hardware /home/hlofving/Downloads/arduino-1.8.19/hardware -tools /home/hlofving/Downloads/arduino-1.8.19/tools-builder -tools /home/hlofving/Downloads/arduino-1.8.19/hardware/tools/avr -built-in-libraries /home/hlofving/Downloads/arduino-1.8.19/libraries -libraries /home/hlofving/Arduino/libraries -fqbn=teensy:avr:teensy40:usb=serial,speed=600,opt=o2std,keys=en-us -vid-pid=16C0_0483 -ide-version=10819 -build-path /tmp/arduino_build_418244 -warnings=none -build-cache /tmp/arduino_cache_75974 -verbose /home/hlofving/see-gh/CLUE_Arcade/CLUE_Arcade/CLUE_Arcade.ino

flash_arduino:
	/home/hlofving/Downloads/arduino-1.8.19/hardware/teensy/../tools/teensy_post_compile -file=CLUE_Arcade.ino -path=/tmp/arduino_build_418244 -tools=/home/hlofving/Downloads/arduino-1.8.19/hardware/teensy/../tools -board=TEENSY40 -reboot -port=/dev/ttyACM0 -portlabel=/dev/ttyACM0 -portprotocol=serial

LDLIBS=-lX11 -lm -lpthread -lstdc++
CCFLAGS=-pthread
arcade_sim: arduino_stub.o arcade_sim.c

