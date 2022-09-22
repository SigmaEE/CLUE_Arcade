#ifndef _TETRIS_H
#define _TETRIS_H

void tetris_loop(void);
void tetris_setup();

int try_move_piece(struct piece * p, int dy, int dx);

struct arcade_screen tetris_screen = {
  .name = (char*)"TETRIS",
  .loop_fn = &tetris_loop,
  .setup_fn = &tetris_setup
};

#endif
