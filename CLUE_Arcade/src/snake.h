#ifndef _SNAKE_H
#define _SNAKE_H

void snake_setup();
void snake_loop();

struct arcade_screen snake_screen = {
  .name = (char*)"SNAKE",
  .loop_fn = &snake_loop,
  .setup_fn = &snake_setup
};

#endif

