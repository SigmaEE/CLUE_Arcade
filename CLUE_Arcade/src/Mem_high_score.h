#ifndef _MEM_HIGH_SCORE_H
#define _MEM_HIGH_SCORE_H

struct hs_player {
  char name[4]={'-','-','-','-'};
  int score=0;
};

struct hs_board {
  struct hs_player scores[10];
};

void clear_high_score(String game_choice);
void re_arrange_board(int position, struct hs_board &board);
void read_high_score_board(String game_choice, struct hs_board &board);
void write_high_score(String game_choice, struct hs_player &player, struct hs_board &board);
int read_high_score_top(String game_choice);
int read_high_score_low(String game_choice);

void print_hs_mem(String game_choice);
void clear_all_memory();
void print_all_memory();

#endif
