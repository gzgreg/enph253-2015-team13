#include "constants.h"

void fireCatapult();
void encodedMotion(bool, int,  bool, int);

void fireCatapult(){
  BASKET_RELEASE.write(0);
  state = TAPE_FOLLOW;
}

void encodedMotion(bool lForward, int lRot, bool rForward, int rRot){
  motor.stop_all();
  delay(500);
  int leftRotations = 0;
  int rightRotations = 0;
  int lSpeed; if(lForward) lSpeed = 100; else lSpeed = -100;
  int rSpeed; if(rForward) rSpeed = -100; else rSpeed = 100;
  bool lOn = digitalRead(0);
  bool rOn = digitalRead(1);
  motor.speed(LEFT_MOTOR, lSpeed);
  motor.speed(RIGHT_MOTOR, rSpeed);
  while(leftRotations< lRot || rightRotations< rRot){
    if(digitalRead(0) && !lOn){
      delay(20);
      if(digitalRead(0)){
        leftRotations++;
        if(leftRotations>= lRot) motor.speed(LEFT_MOTOR, 0);
      }
    } else if(!digitalRead(0)){
      delay(20);
      if(!digitalRead(0)){
        lOn = false;
      }
    }
    
    if(digitalRead(1) && !rOn){
      delay(20);
      if(digitalRead(1)){
        rightRotations++;
        if(rightRotations >= rRot) motor.speed(RIGHT_MOTOR, 0);
      }
    } else if(!digitalRead(1)){
      delay(20);
      if(!digitalRead(1)){
        rOn = false;
      }
    }
  }
}
