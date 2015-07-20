#include "constants.h"

void irFollow();

void irFollow(){
  int i = 0;
  while(state == IR_FOLLOW){
    int prevErr = 0, errTime = 1, prevErrTime = 0;
    int leftErr = analogRead(LEFT_IR);
    int rightErr = analogRead(RIGHT_IR);
    
    int totalErr = (leftErr - rightErr) * 100 / (leftErr + rightErr); //normalize
  
    if (totalErr != prevErr) {
      errTime = 0;
      prevErrTime = errTime; //take slope from change before previous change
    }
  
    int errDeriv = (totalErr - prevErr) / (errTime + prevErrTime);
    int correct = (PIR.Value * totalErr - DIR.Value * errDeriv);
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
          motor.speed(RIGHT_MOTOR, 0);
          Menu();
        }
      }
      
      if(stopbutton()){
        delay(50);
        if(stopbutton()){
          motor.speed(LEFT_MOTOR, 0);
          motor.speed(RIGHT_MOTOR, 0);
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
      sprintf(buffer, "%d %d %f %d", leftErr, rightErr, totalErr, PIR.Value);
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