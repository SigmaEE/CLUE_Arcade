# define CONSOLE_INPUT
# include <time.h>

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/ioctl.h>
# include <termios.h>
#define POSIX_BUILD 1
#define true 1
#define false 0
#define ACT_NONE 0
#define ACT_L  (1 << 0)
#define ACT_R  (1 << 1)
#define ACT_D  (1 << 2)
#define ACT_U (1 << 3)
#define ACT_RN (1 << 4)
#define ACT_Q  (1 << 5)

int speed_delay = 1000;
int score = 0;
int GameOver = 0;
int highScore = 0;

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
#include "tetris.ino"

void clearScreen()
{
	return 0;
}

int main(int argc, char * argv[])
{
	tetris_setup();
	while (1) {
		tetris_loop();
		usleep(100);
	}
}

