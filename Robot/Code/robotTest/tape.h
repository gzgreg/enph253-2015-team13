//#include "constants.h"
//
//void tapeFollow();
//
//void tapeFollow(){
//  int i = 0;
//  int errDeriv, prevErr = 0, dErr = 0, errTime = 1, prevErrTime = 0;
//  unsigned long 
//  lastMarkTime = millis();
//  int32_t errInt = 0;
//  while(state == TAPE_FOLLOW){
//    int leftErr = (analogRead(LEFT_SENSOR) < Thresh.Value) ? 1 : 0;
//    int rightErr = (analogRead(RIGHT_SENSOR) < Thresh.Value) ? 1 : 0;
//    int leftMark = (analogRead(L_MARK_SENSOR) < Thresh.Value) ? 1 : 0;
//    int rightMark = (analogRead(R_MARK_SENSOR) < Thresh.Value) ? 1 : 0;
//  
//    int totalErr = leftErr - rightErr;
//    
//    if (leftErr == 1 && rightErr == 1) {
//      //off line: set error to be same sign as previous error
//      if (prevErr < 0){
//        totalErr = -3;
//      }
//      if (prevErr >= 0){
//        totalErr = 3;
//      }
//    }
//    
//    if(leftMark == 0){
//      if(rightMark == 0 || leftErr == 0 || rightErr == 0){
//        if(!onMarking && (millis() - lastMarkTime) > 2000){
//          state = PET_PICKUP; //only switch state if not already on marking
//          onMarking = true;
//        }
//      } else {
//        totalErr = -10;
//        onMarking = false;
//      }
//    } else if(rightMark == 0){
//      if(leftErr == 0 || rightErr == 0){
//        if(!onMarking && (millis() - lastMarkTime) > 2000){
//          state = PET_PICKUP;
//          onMarking = true;
//        }
//      } else {
//        totalErr = 10;
//        onMarking = false;
//      }
//    } else {
//      onMarking = false;
//    }
//  
//    if (totalErr != prevErr) {
//      errTime = 0;
//      prevErrTime = errTime; //take slope from change before previous change
//      dErr = totalErr - prevErr;
//    }
//    
//    errDeriv = (int32_t)dErr * 100 * DTape.Value / (errTime + prevErrTime);
//    errInt = errInt + totalErr;
//    
//    if(errInt > 3000) errInt = 3000;
//    if(errInt < -3000) errInt = -3000;
//    
//    int correct = (PTape.Value * totalErr + errDeriv + errInt * ITape.Value / 10000);
//    if(correct > Speed.Value * 2) correct = Speed.Value * 2;
//    if(correct < -Speed.Value * 2) correct = -Speed.Value * 2;
//    
//    int leftMotor, rightMotor;
//    leftMotor = (correct > 0) ? Speed.Value : Speed.Value + correct;
//    rightMotor = (correct < 0) ? -Speed.Value : -Speed.Value + correct;
//    
//    motor.speed(LEFT_MOTOR, leftMotor);
//    motor.speed(RIGHT_MOTOR, rightMotor);
//    
//    if(i == 100){
//      if(startbutton()){
//        delay(50);
//        if(startbutton()){
//          motor.speed(LEFT_MOTOR, 0);
//          motor.speed (RIGHT_MOTOR, 0);
//          Menu();
//        }
//      }
//      
//      if(stopbutton()){
//        delay(50);
//        if(stopbutton()){
//          motor.speed(LEFT_MOTOR, 0);
//          motor.speed (RIGHT_MOTOR, 0);
//          delay(BUTTON_WAIT);
//          while(!stopbutton()){
//            int leftSensor = analogRead(LEFT_SENSOR);
//            int rightSensor = analogRead(RIGHT_SENSOR);
//            LCD.clear();
//            LCD.home();
//            LCD.print(leftSensor);
//            LCD.setCursor(0, 1);
//            LCD.print(rightSensor);
//            delay(50);
//          }  
//        }
//        delay(BUTTON_WAIT);
//      }
//      
//      LCD.clear();
//      LCD.home();
//      char buffer[1024];
//      sprintf(buffer, "%d %d %d", errInt, errInt * ITape.Value /10000, errTime+prevErrTime);
//      LCD.print(buffer);
//      LCD.setCursor(0, 1);
//      sprintf(buffer, "%d %d %d", millis() - lastMarkTime, rightMark, correct);
//      LCD.print(buffer);
//      i = 0;
//    }
//    
//    prevErr = totalErr;
//    errTime = errTime + 1;
//    i = i + 1;
//  }
//  motor.stop_all();
//}
//

