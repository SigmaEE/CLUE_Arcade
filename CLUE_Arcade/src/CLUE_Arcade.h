#ifndef _SRC_CLUE_ARCADE_H
#define _SRC_CLUE_ARCADE_H

struct arcade_screen {
  char * name;
  void (*loop_fn)(void);
  void (*setup_fn)(void);
} ;

void draw_word(int x, int y, char * str, uint32_t);

#endif
