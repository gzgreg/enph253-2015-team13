#include "constants.h"

void tapeFollow();

void tapeFollow(){
  int i = 0;
  while(state == TAPE_FOLLOW){
    int errDeriv, prevErr = 0, errTime = 1, prevErrTime = 0;
    int leftErr = (analogRead(LEFT_SENSOR) < Thresh.Value) ? 1 : 0;
    int rightErr = (analogRead(RIGHT_SENSOR) < Thresh.Value) ? 1 : 0;
    int leftMark = (analogRead(L_MARK_SENSOR) < Thresh.Value) ? 1 : 0;
    int rightMark = (analogRead(R_MARK_SENSOR) < Thresh.Value) ? 1 : 0;
  
    int totalErr = leftErr - rightErr;
    
    if (leftErr == 1 && rightErr == 1) {
      //off line: set error to be same sign as previous error
      if (prevErr < 0) totalErr = -3;
      if (prevErr >= 0) totalErr = 3;
    }
    
    if(leftMark == 1){
      if(rightMark == 1 || leftErr == 1 || rightErr == 1){
        if(!onMarking){
          state = PET_PICKUP; //only switch state if not already on marking
          onMarking = true;
        }
      } else {
        totalErr = 10;
        onMarking = false;
      }
    } else if(rightMark == 1){
      if(leftErr == 1 || rightErr == 1){
        if(!onMarking){
          state = PET_PICKUP;
          onMarking = true;
        }
      } else {
        totalErr = -10;
        onMarking = false;
      }
    } else {
      onMarking = false;
    }
  
    if (totalErr != prevErr) {
      errTime = 0;
      prevErrTime = errTime; //take slope from change before previous change
    }
  
    errDeriv = (totalErr - prevErr) / (errTime + prevErrTime);
    int correct = (PTape.Value * totalErr - DTape.Value * errDeriv);
    if(correct > Speed.Value * 2) correct = Speed.Value * 2;
    if(correct < -Speed.Value * 2) correct = -Speed.Value * 2;
    
    int leftMotor, rightMotor;
    leftMotor = (correct > 0) ? Speed.Value : Speed.Value + correct;
    rightMotor = (correct < 0) ? -Speed.Value : -Speed.Value + correct;
    
    motor.speed(LEFT_MOTOR, leftMotor);
    motor.speed(RIGHT_MOTOR, rightMotor);
    
    if(i == 100){
      if(startbutton()){
        delay(50);
        if(startbutton()){
          motor.speed(LEFT_MOTOR, 0);
          motor.speed (RIGHT_MOTOR, 0);
          Menu();
        }
      }
      
      if(stopbutton()){
        delay(50);
        if(stopbutton()){
          motor.speed(LEFT_MOTOR, 0);
          motor.speed (RIGHT_MOTOR, 0);
          delay(BUTTON_WAIT);
          while(!stopbutton()){
            int leftSensor = analogRead(LEFT_SENSOR);
            int rightSensor = analogRead(RIGHT_SENSOR);
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
      sprintf(buffer, "%d %d %d", Speed.Value, PTape.Value, DTape.Value);
      LCD.print(buffer);
      LCD.setCursor(0, 1);
      sprintf(buffer, "%d %d %d", correct, leftMotor, rightMotor);
      LCD.print(buffer);
      i = 0;
    }
    
    prevErr = totalErr;
    errTime = errTime + 1;
    i = i + 1;
  }
}


