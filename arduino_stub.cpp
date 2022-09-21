//#define CONSOLE_INPUT
#include <math.h>
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

void delay(int d)
{
	// XXX what if nanosleep() is interrupted?
	struct timespec delay = { 0, 1000 * 1000 * d };
	nanosleep(&delay, NULL);
}

struct timespec start_time;

int time_delta(struct timespec * t1, struct timespec * t2)
{
  // return delta in milliseconds
  int delta = 0;
  delta = 1000 * (t2->tv_sec - t1->tv_sec);
  delta += (t2->tv_nsec - t1->tv_nsec) / (1000 * 1000);;

  return delta;
}

// XXX relies on start_time being set when arduino starts!
int millis(void)
{
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time_delta(&start_time, &time);
}

extern volatile int joystickX;
extern volatile int joystickY;
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
	int range = h - l;
	return l + random() % range;
}

typedef std::string String;

void setting_screen_loop(void);
void setting_screen_setup(void);

#include "CLUE_Arcade/CLUE_Arcade.ino"
#include "CLUE_Arcade/input_screen.ino"
#include "CLUE_Arcade/Mem_high_score.ino"
#include "CLUE_Arcade/Letters_and_signs.ino"
#include "CLUE_Arcade/Setting_screen.ino"
#include "CLUE_Arcade/snake.ino"
#include "CLUE_Arcade/tetris.ino"
#include "CLUE_Arcade/home_screen.ino"

