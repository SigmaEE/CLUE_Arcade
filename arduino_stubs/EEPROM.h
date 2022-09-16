#ifndef __ARDUINO_STUBS_EEPROM_H
#define __ARDUINO_STUBS_EEPROM_H

struct {
	int read(int) {
		//printf("eeprom read\n");
		return 0;
	}
	void write(int, int) {
		//printf("eeprom write\n");
	}
} EEPROM;

#endif
