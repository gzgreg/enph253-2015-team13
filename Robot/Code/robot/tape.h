#include "constants.h"
#include "catapult.h"

void tapeFollow();
void tapeSearch(bool, int);

void tapeFollow(){
  int i = 0;
  int errDeriv, prevErr = 0, dErr = 0, errTime = 1, prevErrTime = 0;
  unsigned long lastMarkTime = millis();
  int32_t errInt = 0;
  bool tapeEnd = false;
  int div;
  switch(state){
    case TAPE_FOLLOW_DOWN: 
      div = 2;
      break; 
    default: div = 1;
  }
  while(state == TAPE_FOLLOW_UP || state == TAPE_FOLLOW_DOWN){
    int leftErr = (analogRead(LEFT_SENSOR) < Thresh.Value);
    int rightErr = (analogRead(RIGHT_SENSOR) < Thresh.Value);
    int leftMark = (analogRead(L_MARK_SENSOR) < Thresh.Value);
    int rightMark = (analogRead(R_MARK_SENSOR) < Thresh.Value);
    unsigned long currTime = millis();
    
    int totalErr = leftErr - rightErr;
    
    if (leftErr == 1 && rightErr == 1) {
      //off line: set error to be same sign as previous error
      if (prevErr < 0){
        totalErr = -3;
      }
      if (prevErr >= 0){
        totalErr = 3;
      }
      if(tapeEnd){
        state = TURN_AROUND;
        break;
      }
    }
    
    if(leftMark == 0){
      if(analogRead(L_MARK_SENSOR) >= Thresh.Value){
        if(analogRead(L_MARK_SENSOR) >= Thresh.Value){
          if(rightMark == 0 || leftErr < 2 || rightErr < 2 || totalErr > 0){
            if(!onMarking && (currTime - lastMarkTime) > 1500 && (passedMarkings >= 6 || passedMarkings == 3 || passedMarkings == 4)){
              passedMarkings++;
              lastMarkTime = currTime;
              if(state == TAPE_FOLLOW_DOWN){
              } else {
                if(passedMarkings == 4){
                  tapeEnd = true;
                }
              }
              onMarking = true;
              switch(passedMarkings){
                case 7:
                  div = 2;
                  break;
                case 8:
                  div = 1;
                  break;
              }
            }
          } else {
            //totalErr = 20;
            onMarking = false;
          }
        }
      }
      
    } else if(rightMark == 0){
      if(analogRead(R_MARK_SENSOR) >= Thresh.Value){
        if(analogRead(R_MARK_SENSOR) >= Thresh.Value){
          if(leftErr < 2 || rightErr < 2 || totalErr < 0){
            if(!onMarking && (currTime - lastMarkTime) > 1500 && (passedMarkings < 3 || passedMarkings == 5 || passedMarkings == 4)){
              passedMarkings++;
              lastMarkTime = currTime;
              if(state == TAPE_FOLLOW_DOWN){
              } else {
                if(passedMarkings == 4){
                  tapeEnd = true;
                }
              }
              onMarking = true;
              switch(passedMarkings){
                case 1:
                  div = 2;
                  break;
                case 2:
                  div = 1;
                  encodedMotion(true, 3, true, 13);
                  tapeSearch(true, TAPE_FOLLOW_UP);
                  break;
                case 6:
                  div = 2;
                  break;
              }
            }
          } else {
            //totalErr = -20;
            onMarking = false;
          }
        }
      }
    } else {
      onMarking = false;
    }
    
    if (totalErr != prevErr) {
      errTime = 0;
      prevErrTime = errTime; //take slope from change before previous change
    }
    dErr = totalErr - prevErr;
    
    errInt = errInt + totalErr;
    
    if(errInt > 3000) errInt = 3000;
    if(errInt < -3000) errInt = -3000;
    
    int correct = (PTape.Value * totalErr + dErr * DTape.Value*10 / (errTime + prevErrTime));
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
            int leftSensor = analogRead(LEFT_SENSOR);
            int rightSensor = analogRead(RIGHT_SENSOR);
            int lMarkSensor = analogRead(L_MARK_SENSOR);
            int rMarkSensor = analogRead(R_MARK_SENSOR);
            LCD.clear();
            LCD.home();
            char buffer[1024];
            sprintf(buffer, "%d %d", leftSensor, rightSensor);
            LCD.print(buffer);
            LCD.setCursor(0, 1);
            sprintf(buffer, "%d %d", lMarkSensor, rMarkSensor);
            LCD.print(buffer);
            delay(50);
          }  
        }
        delay(BUTTON_WAIT);
      }
      i = 0;
      //LCD.clear(); LCD.home(); LCD.print(passedMarkings); delay(15);
      //hold arm in place
      motor.speed(ARM_BASE, (analogRead(ARM_POT_BASE) - baseAngle));
    }
    
    prevErr = totalErr;
    errTime = errTime + 1;
    i = i + 1;
  }
  motor.stop_all();
}

void tapeSearch(bool left, int stat){
  unsigned long currTime = millis();
  unsigned long initTime = currTime;
  unsigned long switchTime = currTime + 1000;
  while(analogRead(LEFT_SENSOR) < Thresh.Value && analogRead(RIGHT_SENSOR) < Thresh.Value && millis() - initTime < 3500){
    if(left){
      motor.speed(LEFT_MOTOR, -SEARCH_SPD);
      motor.speed(RIGHT_MOTOR, -SEARCH_SPD);
    } else {
      motor.speed(LEFT_MOTOR, SEARCH_SPD);
      motor.speed(RIGHT_MOTOR, SEARCH_SPD);
    }
    if(millis() > switchTime){
      left = !left;
      currTime = millis();
      switchTime = currTime + 2000;
    }
  }
  motor.speed(LEFT_MOTOR, 0);
  motor.speed(RIGHT_MOTOR, 0);
  state = stat;
}

