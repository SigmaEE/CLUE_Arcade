//#define CONSOLE_INPUT
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <termios.h>
#include <string>
#include <semaphore.h>

extern sem_t draw_done_sem;
extern sem_t start_draw_sem;
extern int draw_counter;

// make these callable from pure C
extern "C" void setup(void);
extern "C" void loop(void);

// arduino compatibility stuff
#define true 1
#define false 0
#define bool int
#define WS2812B 0
#define GRB 0
#define TypicalSMD5050 0
#define A0 0
#define A1 1
#define ACT_NONE 0
#define ACT_L  (1 << 0)
#define ACT_R  (1 << 1)
#define ACT_D  (1 << 2)
#define ACT_U (1 << 3)
#define ACT_RN (1 << 4)
#define ACT_Q  (1 << 5)

int millis(void);
// fastled
typedef struct CRGB {
	int r;
	int g;
	int b;
	inline CRGB& operator= (const uint32_t colorcode) __attribute__((always_inline))
	{
		r = (colorcode >> 16) & 0xFF;
		g = (colorcode >>  8) & 0xFF;
		b = (colorcode >>  0) & 0xFF;
		return *this;
	}
	typedef enum {
		AliceBlue=0xF0F8FF,
		Amethyst=0x9966CC,
		AntiqueWhite=0xFAEBD7,
		Aqua=0x00FFFF,
		Aquamarine=0x7FFFD4,
		Azure=0xF0FFFF,
		Beige=0xF5F5DC,
		Bisque=0xFFE4C4,
		Black=0x000000,
		BlanchedAlmond=0xFFEBCD,
		Blue=0x0000FF,
		BlueViolet=0x8A2BE2,
		Brown=0xA52A2A,
		BurlyWood=0xDEB887,
		CadetBlue=0x5F9EA0,
		Chartreuse=0x7FFF00,
		Chocolate=0xD2691E,
		Coral=0xFF7F50,
		CornflowerBlue=0x6495ED,
		Cornsilk=0xFFF8DC,
		Crimson=0xDC143C,
		Cyan=0x00FFFF,
		DarkBlue=0x00008B,
		DarkCyan=0x008B8B,
		DarkGoldenrod=0xB8860B,
		DarkGray=0xA9A9A9,
		DarkGrey=0xA9A9A9,
		DarkGreen=0x006400,
		DarkKhaki=0xBDB76B,
		DarkMagenta=0x8B008B,
		DarkOliveGreen=0x556B2F,
		DarkOrange=0xFF8C00,
		DarkOrchid=0x9932CC,
		DarkRed=0x8B0000,
		DarkSalmon=0xE9967A,
		DarkSeaGreen=0x8FBC8F,
		DarkSlateBlue=0x483D8B,
		DarkSlateGray=0x2F4F4F,
		DarkSlateGrey=0x2F4F4F,
		DarkTurquoise=0x00CED1,
		DarkViolet=0x9400D3,
		DeepPink=0xFF1493,
		DeepSkyBlue=0x00BFFF,
		DimGray=0x696969,
		DimGrey=0x696969,
		DodgerBlue=0x1E90FF,
		FireBrick=0xB22222,
		FloralWhite=0xFFFAF0,
		ForestGreen=0x228B22,
		Fuchsia=0xFF00FF,
		Gainsboro=0xDCDCDC,
		GhostWhite=0xF8F8FF,
		Gold=0xFFD700,
		Goldenrod=0xDAA520,
		Gray=0x808080,
		Grey=0x808080,
		Green=0x008000,
		GreenYellow=0xADFF2F,
		Honeydew=0xF0FFF0,
		HotPink=0xFF69B4,
		IndianRed=0xCD5C5C,
		Indigo=0x4B0082,
		Ivory=0xFFFFF0,
		Khaki=0xF0E68C,
		Lavender=0xE6E6FA,
		LavenderBlush=0xFFF0F5,
		LawnGreen=0x7CFC00,
		LemonChiffon=0xFFFACD,
		LightBlue=0xADD8E6,
		LightCoral=0xF08080,
		LightCyan=0xE0FFFF,
		LightGoldenrodYellow=0xFAFAD2,
		LightGreen=0x90EE90,
		LightGrey=0xD3D3D3,
		LightPink=0xFFB6C1,
		LightSalmon=0xFFA07A,
		LightSeaGreen=0x20B2AA,
		LightSkyBlue=0x87CEFA,
		LightSlateGray=0x778899,
		LightSlateGrey=0x778899,
		LightSteelBlue=0xB0C4DE,
		LightYellow=0xFFFFE0,
		Lime=0x00FF00,
		LimeGreen=0x32CD32,
		Linen=0xFAF0E6,
		Magenta=0xFF00FF,
		Maroon=0x800000,
		MediumAquamarine=0x66CDAA,
		MediumBlue=0x0000CD,
		MediumOrchid=0xBA55D3,
		MediumPurple=0x9370DB,
		MediumSeaGreen=0x3CB371,
		MediumSlateBlue=0x7B68EE,
		MediumSpringGreen=0x00FA9A,
		MediumTurquoise=0x48D1CC,
		MediumVioletRed=0xC71585,
		MidnightBlue=0x191970,
		MintCream=0xF5FFFA,
		MistyRose=0xFFE4E1,
		Moccasin=0xFFE4B5,
		NavajoWhite=0xFFDEAD,
		Navy=0x000080,
		OldLace=0xFDF5E6,
		Olive=0x808000,
		OliveDrab=0x6B8E23,
		Orange=0xFFA500,
		OrangeRed=0xFF4500,
		Orchid=0xDA70D6,
		PaleGoldenrod=0xEEE8AA,
		PaleGreen=0x98FB98,
		PaleTurquoise=0xAFEEEE,
		PaleVioletRed=0xDB7093,
		PapayaWhip=0xFFEFD5,
		PeachPuff=0xFFDAB9,
		Peru=0xCD853F,
		Pink=0xFFC0CB,
		Plaid=0xCC5533,
		Plum=0xDDA0DD,
		PowderBlue=0xB0E0E6,
		Purple=0x800080,
		Red=0xFF0000,
		RosyBrown=0xBC8F8F,
		RoyalBlue=0x4169E1,
		SaddleBrown=0x8B4513,
		Salmon=0xFA8072,
		SandyBrown=0xF4A460,
		SeaGreen=0x2E8B57,
		Seashell=0xFFF5EE,
		Sienna=0xA0522D,
		Silver=0xC0C0C0,
		SkyBlue=0x87CEEB,
		SlateBlue=0x6A5ACD,
		SlateGray=0x708090,
		SlateGrey=0x708090,
		Snow=0xFFFAFA,
		SpringGreen=0x00FF7F,
		SteelBlue=0x4682B4,
		Tan=0xD2B48C,
		Teal=0x008080,
		Thistle=0xD8BFD8,
		Tomato=0xFF6347,
		Turquoise=0x40E0D0,
		Violet=0xEE82EE,
		Wheat=0xF5DEB3,
		White=0xFFFFFF,
		WhiteSmoke=0xF5F5F5,
		Yellow=0xFFFF00,
		YellowGreen=0x9ACD32,

		// LED RGB color that roughly approximates
		// the color of incandescent fairy lights,
		// assuming that you're using FastLED
		// color correction on your LEDs (recommended).
		FairyLight=0xFFE42D,
		// If you are using no color correction, use this
		FairyLightNCC=0xFF9D2A

	} HTMLColorCode;
} CRGB;

typedef struct CLEDController {
	void setCorrection(int i) {
		printf("CLEDController.setCorrection()\n");
	}
} CLEDController;

extern CRGB leds_plus_safety_pixel[ 58 * 30 + 1 ] ;
struct CRGB * fastled_crgb;
int fastled_nleds;
int fastled_leds[600 * 30];
CLEDController c;
struct FastLED {
	void show(void) {
		//printf("FastLED.show()\n");
		for (int i = 0; i < (60 * 30); i++) {
			fastled_leds[i] = leds_plus_safety_pixel[i].b;
			fastled_leds[i] += leds_plus_safety_pixel[i].g << 8;
			fastled_leds[i] += leds_plus_safety_pixel[i].r << 16;
		}
		draw_counter++;
		int t = millis();
		sem_post(&start_draw_sem);
		sem_wait(&draw_done_sem);
		printf("time %d\n", millis() - t);
	}
	void setBrightness( int i ) {
		printf("FastLED.setBrightness\n");
	}

	template<int a, uint8_t DATA_PIN, int RGB_ORDER> static CLEDController &addLeds(struct CRGB *data, int nLedsOrOffset) {
		fastled_crgb = data;
		return c;
	}
} FastLED;

struct serial {
	void println(const std::string s) {
		printf("%s\n", s.c_str());
	}
	void println(int i) {
		printf("%d\n", i);
	}
	void print(const std::string s) {
		printf("%s", s.c_str());
	}
	void print(int i) {
		printf("%d", i);
	}
	void begin(int) {
		printf("Serial.begin()\n");
	}
};

struct serial Serial;
struct serial Serial1;

struct {
	int read(int) {
		//printf("eeprom read\n");
		return 0;
	}
	void write(int, int) {
		//printf("eeprom write\n");
	}
} EEPROM;

void delay(int d)
{
	struct timespec delay = { 0, 1000 * 1000 * d };
	struct timespec left;
	nanosleep(&delay, &left);
}

/*
int check_input()
{
	int action = ACT_NONE;

	long int n = 0;
	// if n == 0 there is nothing to read
	ioctl(STDIN_FILENO, FIONREAD, &n);
	while (n > 0) {
		int c = fgetc(stdin);
		switch (c) {
			case '1': action |= ACT_L; break;
			case '2': action |= ACT_R; break;
			case '3': action |= ACT_D; break;
			case '8': action |= ACT_RN; break;
			case '9': action |= ACT_U; break;
			case 'q': action |= ACT_Q; break;
		}
		ioctl(STDIN_FILENO, FIONREAD, &n);
	}
	return action;
  // compare current input to new input.
  // if new input then use it
  // if repeating input (holds left, right or down)
  // then dont use it until after some time has passed
}
*/

struct timespec start_time;

int time_delta(struct timespec * t1, struct timespec * t2)
{
  // return delta in milliseconds
  int delta = 0;
  delta = 1000 * (t2->tv_sec - t1->tv_sec);
  delta += (t2->tv_nsec - t1->tv_nsec) / (1000 * 1000);;

  return delta;
}

// relies on start_time being set when game starts!
int millis(void)
{
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time_delta(&start_time, &time);
}

extern int joystickX;
extern int joystickY;
int analogRead(int pin)
{
	//return 100;
	if (pin == A0) {
		// xPotPin = A0
		if (joystickX < 0) {
			return 200;
		} else if (joystickX > 0) {
			return 800;
		} else {
			return 500;
		}
	}
	if (pin == A1) {
		if (joystickY < 0) {
			return 800;
		} else if (joystickY > 0) {
			return 200;
		} else {
			return 500;
		}
		// yPotPin = A1
	}
	return 0;
}

int random(int l, int h)
{
	printf("XXXXXX fix random!!!11\n");
	Serial.println("aoeuao");
	return 0;
}

typedef std::string String;

void setting_screen(void);

#include "CLUE_Arcade/CLUE_Arcade.ino"
#include "CLUE_Arcade/Letters_and_signs.ino"
#include "CLUE_Arcade/Setting_screen.ino"
#include "CLUE_Arcade/snake.ino"
#include "CLUE_Arcade/tetris.ino"
#include "CLUE_Arcade/home_screen.ino"

