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
    template< typename T > T &get( int idx, T &t ){
        #if defined(__has_include) && __has_include(<type_traits>)
        static_assert(std::is_trivially_copyable<T>::value,"You can not use this type with EEPROM.get" ); // the code below only makes sense if you can "memcpy" T
        #endif
        int e = idx;
        uint8_t *ptr = (uint8_t*) &t;
        for( int count = sizeof(T) ; count ; --count, ++e ) { /* *ptr++ = *e; */ }
        return t;
    }

    template< typename T > const T &put( int idx, const T &t ){
        #if defined(__has_include) && __has_include(<type_traits>)
        static_assert(std::is_trivially_copyable<T>::value, "You can not use this type with EEPROM.put"); // the code below only makes sense if you can "memcpy" T
        #endif
        const uint8_t *ptr = (const uint8_t*) &t;
        //eeprom_write_block(ptr, (void *)idx, sizeof(T));
	int e = 0;
        for( int count = sizeof(T) ; count ; --count, ++e ) { /* (*e).update( *ptr++ ); */ }
        return t;
    }
} EEPROM;

#endif
