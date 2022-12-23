#include "src/joystick_input.h"

int check_joystick_input()
{
  int action = ACT_NONE;
  static long unsigned int current_time;
  static long unsigned int last_up[2] = {0};
  static long unsigned int last_down[2] = {0};
  static long unsigned int last_left[2] = {0};
  static long unsigned int last_right[2] = {0};
  static long unsigned int last_button1[2] = {0};
  static long unsigned int last_button2[2] = {0};
  
  int p1Right = digitalRead(player1Right);
  int p1Up = digitalRead(player1Up);
  int p1Left = digitalRead(player1Left);
  int p1Down = digitalRead(player1Down);
  int p1B1 = digitalRead(player1B1);
  int p1B2 = digitalRead(player1B2);
 

  current_time = millis();

  if ( p1Left == 0) {
    if (current_time - last_left[0] > left_delay) {
      action |= ACT_L_P1;
      last_left[0] = millis();
    }
  } else if (p1Right == 0) {
    if (current_time - last_right[0] > right_delay) {
      action |= ACT_R_P1;
      last_right[0] = millis();
    }
  }
  if (p1Down == 0) {
    if (current_time - last_down[0] > down_delay) {
      action |= ACT_D_P1;
      last_down[0] = millis();
    }
  } else if (p1Up == 0) {

    if (current_time - last_up[0] > up_delay) {
      action |= ACT_U_P1;
      last_up[0] = millis();
    }
  }
  if (p1B1 == 1){
    if (current_time - last_button1[0] > b1_delay){
      action |= ACT_B1_P1;
      last_button1[0] = millis(); 
    }
  } else if (p1B2 == 1){
    if (current_time - last_button2[0] > b2_delay){
      action |= ACT_B2_P1;
      last_button2[0] = millis(); 
    }
  }

  int p2Right = digitalRead(player2Right);
  int p2Up = digitalRead(player2Up);
  int p2Left = digitalRead(player2Left);
  int p2Down = digitalRead(player2Down);
  int p2B1 = digitalRead(player2B1);
  int p2B2 = digitalRead(player2B2);

  if ( p2Left == 0) {
    if (current_time - last_left[1] > left_delay) {
      action |= ACT_L_P2;
      last_left[1] = millis();
    }
  } else if (p2Right == 0) {
    if (current_time - last_right[1] > right_delay) {
      action |= ACT_R_P2;
      last_right[1] = millis();
    }
  }
  if (p2Down == 0) {
    if (current_time - last_down[1] > down_delay) {
      action |= ACT_D_P2;
      last_down[1] = millis();
    }
  } else if (p2Up == 0) {

    if (current_time - last_up[1] > up_delay) {
      action |= ACT_U_P2;
      last_up[1] = millis();
    }
  }
  if (p2B1 == 1){
    if (current_time - last_button1[1] > b1_delay){
      action |= ACT_B1_P2;
      last_button1[1] = millis(); 
    }
  } else if (p2B2 == 1){
    if (current_time - last_button2[1] > b2_delay){
      action |= ACT_B2_P2;
      last_button2[1] = millis(); 
    }
  }
  return action;
}
