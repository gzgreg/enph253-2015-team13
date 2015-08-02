#include "constants.h"

void irFollow();

void irFollow(){
  int i = 0;
  long leftErr = 0, rightErr = 0;
  bool lOn = digitalRead(0);
  bool rOn = digitalRead(1);
  int leftRotations = 0;
  int rightRotations = 0;
  while(state == IR_FOLLOW_F || state == IR_FOLLOW_F){
    long prevErr = 0, errTime = 1, prevErrTime = 0;
    leftErr = (analogRead(LEFT_IR) + leftErr*5) / 6;
    rightErr = (analogRead(RIGHT_IR) + rightErr*5)/6;
    
    long totalErr = (rightErr - leftErr) * 100 / (leftErr + rightErr); //normalize
  
    if (totalErr != prevErr) {
      errTime = 0;
      prevErrTime = errTime; //take slope from change before previous change
    }
  
    long errDeriv = (totalErr - prevErr) / (errTime + prevErrTime);
    long correct = (PIR.Value * totalErr + DIR.Value * errDeriv);
    if(correct > Speed.Value) correct = Speed.Value;
    if(correct < -Speed.Value) correct = -Speed.Value;
    
    int leftMotor, rightMotor;
    leftMotor = (correct > 0) ? Speed.Value *0.75 : Speed.Value *0.75 + correct;
    rightMotor = (correct < 0) ? -Speed.Value *0.75: -Speed.Value *0.75+ correct;
    
    motor.speed(LEFT_MOTOR, leftMotor);
    motor.speed(RIGHT_MOTOR, rightMotor);
    
    //encoding
    if(digitalRead(LEFT_ENCODER) && !lOn){
      delay(20);
      if(digitalRead(LEFT_ENCODER)){
        leftRotations++;
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
      }
    } else if(!digitalRead(RIGHT_ENCODER)){
      delay(20);
      if(!digitalRead(RIGHT_ENCODER)){
        rOn = false;
      }
    }
    
    if(state == IR_FOLLOW_F){
      //check IR magnitude and encoding to find distance to box
    }
    if(state == IR_FOLLOW_B){
      if(analogRead(LEFT_SENSOR) <= Thresh.Value || analogRead(RIGHT_SENSOR) <= Thresh.Value){
        state == TAPE_FOLLOW_DOWN;
      }
      //check tape and distance to fifth pet
    }
    
    if(i == 100){
      if(startbutton()){
        delay(50);
        if(startbutton()){
          motor.speed(LEFT_MOTOR, 0);
          motor.speed(RIGHT_MOTOR, 0);
          Menu();
        }
      }
      
      if(stopbutton()){
        delay(50);
        if(stopbutton()){
          motor.stop_all();
          delay(BUTTON_WAIT);
          while(!stopbutton()){
            int leftSensor = analogRead(LEFT_IR);
            int rightSensor = analogRead(RIGHT_IR);
            LCD.clear();
            LCD.home();
            LCD.print(leftSensor);
            LCD.setCursor(0, 1);
            LCD.print(rightSensor);
            delay(50);
          }  
        }
        delay(BUTTON_WAIT);
      }
      
      LCD.clear();
      LCD.home();
      char buffer[1024];
      sprintf(buffer, "%d %d %d %d", leftErr, rightErr, totalErr, PIR.Value);
      LCD.print(buffer);
      LCD.setCursor(0, 1);
      sprintf(buffer, "%d %d %d %d", correct, leftMotor, rightMotor, DIR.Value);
      LCD.print(buffer);
      i = 0;
    }
    
    prevErr = totalErr;
    errTime = errTime + 1;
    i = i + 1;
  }
}
