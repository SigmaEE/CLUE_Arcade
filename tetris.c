#define CONSOLE_INPUT
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>

#define POSIX_BUILD 1
// arduino compatibility stuff
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
int GameOver_tetris = 0;
int highScore = 0;
int highScore_tetris = 0;

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

void writeHighScore()
{
	return;;
}

void hsAnimation()
{
	return;
}

void deathAnimation()
{
	return;
}

void clearScreen()
{
	return;
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

// relies on start_time being set when game starts!
int millis(void)
{
	struct timespec time;
	clock_gettime(CLOCK_REALTIME, &time);
	return time_delta(&start_time, &time);
}

/*
  void test_game(void)
  {
  for (int i = 0; i < 1; ++i) {
    try_move_piece(&active_piece, 0, (random() % 3) - 1);
    rotate_piece(&active_piece, (random() % 3) - 1);
    if (0 == try_move_piece(&active_piece, 1, 0)) {
      stick_piece(&active_piece);
      new_piece(&active_piece);
      if (0 == try_move_piece(&active_piece, 0, 0)) {
        // game over!
        printf("Game over after %d moves\n", i);
        break;
      }
    }
    render();
    usleep(100000);
  }
  }
*/

#include "tetris.ino"

int main(int argc, char * argv[])
{
	// set up keyboard input
	struct termios original_termios;
	struct termios term_options;
	tcgetattr(STDIN_FILENO, &original_termios);
	tcgetattr(STDIN_FILENO, &term_options);
	tcflag_t of = term_options.c_oflag;
	cfmakeraw(&term_options);
	term_options.c_oflag = of;
	tcsetattr(STDIN_FILENO, TCSANOW, &term_options);

	// initialize time keeping
	clock_gettime(CLOCK_REALTIME, &start_time);

	tetris_setup();
	while (!GameOver_tetris) {
		tetris_loop();
	}

	// restore terminal to normal
	tcsetattr(STDIN_FILENO, TCSANOW, &original_termios);
}

