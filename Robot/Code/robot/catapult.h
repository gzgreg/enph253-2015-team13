#include "constants.h"

void encodedMotion(bool, int,  bool, int);

void encodedMotion(bool lForward, int lRot, bool rForward, int rRot){
  motor.stop_all();
  delay(500);
  int leftRotations = 0;
  int rightRotations = 0;
  int lSpeed; if(lForward) lSpeed = ENCODE_SPD; else lSpeed = -ENCODE_SPD;
  int rSpeed; if(rForward) rSpeed = -ENCODE_SPD; else rSpeed = ENCODE_SPD;
  bool lOn = digitalRead(LEFT_ENCODER);
  bool rOn = digitalRead(RIGHT_ENCODER);
  motor.speed(LEFT_MOTOR, lSpeed);
  motor.speed(RIGHT_MOTOR, rSpeed);
  while(leftRotations< lRot || rightRotations< rRot){
    if(digitalRead(LEFT_ENCODER) && !lOn){
      delay(20);
      if(digitalRead(LEFT_ENCODER)){
        leftRotations++;
        if(leftRotations>= lRot) motor.speed(LEFT_MOTOR, 0);
      }
    } else if(!digitalRead(LEFT_ENCODER)){
      delay(20);
      if(!digitalRead(LEFT_ENCODER)){
        lOn = false;
      }
    }
    
    if(digitalRead(RIGHT_ENCODER) && !rOn){
      delay(20);
      if(digitalRead(RIGHT_ENCODER)){
        rightRotations++;
        if(rightRotations >= rRot) motor.speed(RIGHT_MOTOR, 0);
      }
    } else if(!digitalRead(RIGHT_ENCODER)){
      delay(20);
      if(!digitalRead(RIGHT_ENCODER)){
        rOn = false;
      }
    }
  }
}
