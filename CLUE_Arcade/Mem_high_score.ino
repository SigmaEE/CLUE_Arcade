#define SNAKE_ADR_START 0
#define SNAKE_ADR_END 79
#define TETRIS_ADR_START 100
#define TETRIS_ADR_END 179

#include "src/Mem_high_score.h"

void print_hs_mem(String game_choice)
{
  struct hs_board board;
  read_high_score_board(game_choice, board);

  for (int i = 0; i<10; i++) {
    Serial.print(board.scores[i].name[0]);
    Serial.print(board.scores[i].name[1]);
    Serial.print(board.scores[i].name[2]);
    Serial.print(board.scores[i].name[3]);
    Serial.print("\t");
    Serial.println(board.scores[i].score);
  }
}

void print_all_memory()
{
  for (int i=0; i<1024; i++) {
    Serial.println(EEPROM.read(i));
  }
}

void clear_all_memory()
{
  for (int i=0; i<1024; i++) {
    EEPROM.write(i, 0);
  }
}

void clear_high_score(String game_choice) {
  struct hs_board curr_board;
  int start_adr;
  //clear_all_memory();
  if (game_choice == "t")
    start_adr = TETRIS_ADR_START;
  else if(game_choice == 's')
    start_adr = SNAKE_ADR_START;

  EEPROM.put(start_adr, curr_board);
}

void re_arrange_board(int position, struct hs_board &board)
{
  for (int i=9; i>position; i--) {
    board.scores[i] = board.scores[i-1];
  }
}

void write_high_score(String game_choice, struct hs_player &player, struct hs_board &board)
{

  //read_high_score_board(game_choice, curr_board);
  int new_score = player.score;
  int tmp_score;

  for (int i=0; i<10; i++) {
    tmp_score = board.scores[i].score;
    if (new_score > tmp_score) {
      re_arrange_board(i, board);
      board.scores[i] = player;
      break;
    }
  }

  int start_adr;
  if (game_choice == "t") {
    start_adr = TETRIS_ADR_START;
  }
  else if (game_choice == "s") {
    start_adr = SNAKE_ADR_START;
  }

  EEPROM.put(start_adr, board);
}

void read_high_score_board(String game_choice, struct hs_board &board)
{
  int start_adr;
  if (game_choice == "t") {
    start_adr = TETRIS_ADR_START;
  } else if (game_choice == "s") {
    start_adr = SNAKE_ADR_START;
  }
  EEPROM.get(start_adr, board);
}


int read_high_score_top(String game_choice)
{
  struct hs_board curr_board;
  struct hs_player top_player;
  read_high_score_board(game_choice, curr_board);

  return curr_board.scores[0].score;
}

int read_high_score_low(String game_choice)
{
  struct hs_player low_player;
  struct hs_board curr_board;

  read_high_score_board(game_choice, curr_board);
  for (int i=0; i<10; i++) {
    if (curr_board.scores[i].score == 0) {
      if (i != 0) 
        low_player = curr_board.scores[i-1];
      else
        low_player.score = 0;
    }
    else {
      low_player = curr_board.scores[9];
    }
  }
  return low_player.score;
}
