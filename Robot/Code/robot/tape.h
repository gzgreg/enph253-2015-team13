#include "constants.h"

void tapeFollow();
void tapeSearch();

void tapeFollow(){
  int i = 0;
  int errDeriv, prevErr = 0, dErr = 0, errTime = 1, prevErrTime = 0;
  unsigned long lastMarkTime = millis();
  int32_t errInt = 0;
  int div;
  switch(petNum){
    case 2: 
      div = 2;
      break; 
    default: div = 1;
  }
  while(state == TAPE_FOLLOW_UP || state == TAPE_FOLLOW_DOWN){
    int leftErr = (analogRead(LEFT_SENSOR) < Thresh.Value) ? 1 : 0;
    int rightErr = (analogRead(RIGHT_SENSOR) < Thresh.Value) ? 1 : 0;
    int leftMark = (analogRead(L_MARK_SENSOR) < Thresh.Value) ? 1 : 0;
    int rightMark = (analogRead(R_MARK_SENSOR) < Thresh.Value) ? 1 : 0;
  
    int totalErr = leftErr - rightErr;
    
    if (leftErr == 1 && rightErr == 1) {
      //off line: set error to be same sign as previous error
      if (prevErr < 0){
        totalErr = -3;
      }
      if (prevErr >= 0){
        totalErr = 3;
      }
    }
    
    if(leftMark == 0){
      if(rightMark == 0 || leftErr == 0 || rightErr == 0){
        if(!onMarking && (millis() - lastMarkTime) > 2000){
          passedMarkings++;
          lastMarkTime = millis();
          if(state == TAPE_FOLLOW_DOWN){
            if(passedMarkings > 5) state = PET_PICKUP; //only switch state if not already on marking
          } else {
            if(passedMarkings == 4) state = IR_FOLLOW_F;
          }
          onMarking = true;
        }
      } else {
        totalErr = -10;
        onMarking = false;
      }
    } else if(rightMark == 0){
      if(leftErr == 0 || rightErr == 0){
        if(!onMarking && (millis() - lastMarkTime) > 2000){
          passedMarkings++;
          lastMarkTime = millis();
          if(state == TAPE_FOLLOW_DOWN){
            if(passedMarkings > 5) state = PET_PICKUP; //only switch state if not already on marking
          } else {
            if(passedMarkings == 4) state = IR_FOLLOW_F;
          }
          onMarking = true;
        }
      } else {
        totalErr = 10;
        onMarking = false;
      }
    } else {
      onMarking = false;
    }
  
    if (totalErr != prevErr) {
      errTime = 0;
      prevErrTime = errTime; //take slope from change before previous change
      dErr = totalErr - prevErr;
    }
    
    errDeriv = (int32_t)dErr * 100 * DTape.Value / (errTime + prevErrTime);
    errInt = errInt + totalErr;
    
    if(errInt > 3000) errInt = 3000;
    if(errInt < -3000) errInt = -3000;
    
    int correct = (PTape.Value * totalErr + errDeriv + errInt * ITape.Value / 10000);
    if(correct > Speed.Value * 2) correct = Speed.Value * 2;
    if(correct < -Speed.Value * 2) correct = -Speed.Value * 2;
    
    int leftMotor, rightMotor;
    leftMotor = (correct > 0) ? Speed.Value / div : (Speed.Value + correct) / div;
    rightMotor = (correct < 0) ? -Speed.Value / div : (-Speed.Value + correct) / div;
    
    motor.speed(LEFT_MOTOR, leftMotor);
    motor.speed(RIGHT_MOTOR, rightMotor);
    
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
          motor.speed(LEFT_MOTOR, 0);
          motor.speed (RIGHT_MOTOR, 0);
          delay(BUTTON_WAIT);
          while(!stopbutton()){
            int leftSensor = analogRead(L_MARK_SENSOR);
            int rightSensor = analogRead(R_MARK_SENSOR);
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
    }
    
    prevErr = totalErr;
    errTime = errTime + 1;
    i = i + 1;
  }
  motor.stop_all();
}

void tapeSearch(){
  unsigned long currTime = millis();
  unsigned long initTime = currTime;
  unsigned long switchTime = currTime + 600;
  bool left = true;
  while(analogRead(LEFT_SENSOR) < Thresh.Value && analogRead(RIGHT_SENSOR) < Thresh.Value && millis() - initTime < 2000){
    if(left){
      motor.speed(LEFT_MOTOR, -70);
      motor.speed(RIGHT_MOTOR, -70);
    } else {
      motor.speed(LEFT_MOTOR, 70);
      motor.speed(RIGHT_MOTOR, 70);
    }
    if(millis() > switchTime){
      left = !left;
      currTime = millis();
      switchTime = currTime + 1200;
    }
  }
  motor.speed(LEFT_MOTOR, 0);
  motor.speed(RIGHT_MOTOR, 0);
  state = TAPE_FOLLOW_DOWN;
}

