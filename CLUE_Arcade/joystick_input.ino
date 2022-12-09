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
    if (current_time - last_left[0] > 100) {
      action |= ACT_L_P1;
      Serial.println("p1 LEFT");
      last_left[0] = millis();
    }
  } else if (p1Right == 0) {
    if (current_time - last_right[0] > 100) {
      action |= ACT_R_P1;
      Serial.println("p1 RIGHT");
      last_right[0] = millis();
    }
  }
  if (p1Down == 0) {
    if (current_time - last_down[0] > 50) {
      action |= ACT_D_P1;
      Serial.println("p1 DOWN");
      last_down[0] = millis();
    }
  } else if (p1Up == 0) {

    if (current_time - last_up[0] > 200) {
      action |= ACT_U_P1;
      Serial.println("p1 UP");
      last_up[0] = millis();
    }
  }
  if (p1B1 == 1){
    if (current_time - last_button1[0] > 200){
      action |= ACT_B1_P1;
      Serial.println("p1 Button 1");
      last_button1[0] = millis(); 
    }
  } else if (p1B2 == 1){
    if (current_time - last_button2[0] > 200){
      action |= ACT_B2_P1;
      Serial.println("p1 Button 2");
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
    if (current_time - last_left[1] > 100) {
      action |= ACT_L_P2;
      Serial.println("p2 LEFT");
      last_left[1] = millis();
    }
  } else if (p2Right == 0) {
    if (current_time - last_right[1] > 100) {
      action |= ACT_R_P2;
      Serial.println("p2 RIGHT");
      last_right[1] = millis();
    }
  }
  if (p2Down == 0) {
    if (current_time - last_down[1] > 50) {
      action |= ACT_D_P2;
      Serial.println("p2 DOWN");
      last_down[1] = millis();
    }
  } else if (p2Up == 0) {

    if (current_time - last_up[1] > 200) {
      action |= ACT_U_P2;
      Serial.println("p2 UP");
      last_up[1] = millis();
    }
  }
  if (p2B1 == 1){
    if (current_time - last_button1[1] > 200){
      action |= ACT_B1_P2;
      Serial.println("p2 Button 1");
      last_button1[1] = millis(); 
    }
  } else if (p2B2 == 1){
    if (current_time - last_button2[1] > 200){
      action |= ACT_B2_P2;
      Serial.println("p2 Button 2");
      last_button2[1] = millis(); 
    }
  }
  Serial.print("action: ");
  Serial.println(action);
  return action;
}
