#ifndef __ARDUINO_STUBS_EEPROM_H
#define __ARDUINO_STUBS_EEPROM_H
#if defined(__has_include) && __has_include(<type_traits>)
#include <type_traits>
#endif

#include <stdio.h>

struct {
	int read(int a) {
		uint8_t t;
		get(a, t);
		return t;
	}
	void write(int a, int v) {
		put(a, v);
	}
	template< typename T > T &get( int idx, T &t ){
#if defined(__has_include) && __has_include(<type_traits>)
		static_assert(std::is_trivially_copyable<T>::value,"You can not use this type with EEPROM.get" ); // the code below only makes sense if you can "memcpy" T
#endif
		uint8_t *ptr = (uint8_t*) &t;
		FILE * fd = fopen("./eeprom.bin", "r+");
		if (fd == NULL) {
			fd = fopen("./eeprom.bin", "w+");
	}
	fseek(fd, idx, SEEK_SET);
	int r = fread(ptr, sizeof(T), 1, fd);
	if (r < 1) {
		printf("Dude! File too small\n");
		void * buf = calloc(1, sizeof(T));
		fwrite(buf, sizeof(T), 1, fd);
		memcpy(ptr, buf, sizeof(T));
		free(buf);
	}
	fclose(fd);
        return t;
    }

    template< typename T > const T &put( int idx, const T &t ){
        #if defined(__has_include) && __has_include(<type_traits>)
        static_assert(std::is_trivially_copyable<T>::value, "You can not use this type with EEPROM.put"); // the code below only makes sense if you can "memcpy" T
        #endif
        const uint8_t *ptr = (const uint8_t*) &t;
        //eeprom_write_block(ptr, (void *)idx, sizeof(T));
	FILE * fd = fopen("./eeprom.bin", "r+");
	if (fd == NULL) {
		fd = fopen("./eeprom.bin", "w+");
	}
	fseek(fd, idx, SEEK_SET);
	fwrite(ptr, sizeof(T), 1, fd);
	fclose(fd);
        return t;
    }
} EEPROM;

#endif
