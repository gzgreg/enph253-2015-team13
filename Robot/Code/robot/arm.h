#include "constants.h"

void moveArm(int, int, int);
void armPID(int, int, int, int);
void swingL();
void swingR();
void raise();
void lower();
void releasePet();
void petPickup(int);

/*
Putting -1 into any parameter will cause that piece of the arm to stay stationary.
base and joint1 take pot values, not angles. joint2 takes an angle. These should be found empirically
*/
void moveArm(int base, int joint1, int joint2){
  if(joint2 != -1) ARM_2.write(joint2);
  armPID(ARM_BASE, base, ARM_1, joint1);
  baseAngle = analogRead(ARM_POT_BASE);
  joint1Angle = analogRead(ARM_POT_1);
  joint2Angle = joint2;
}

void armPID(int motorb, int valueb, int motor1, int value1){
  int error1 = analogRead(ARM_POT_BASE) - value1;
  int errorb = analogRead(ARM_POT_1) - valueb;
  
  int prevErr1 = error1;
  int prevErrb = errorb;
  int errIntb = error1;
  int errInt1 = errorb;
  unsigned long t1 = millis();
  unsigned long t2 = t1;
  unsigned long tInitial = t1;
  int dt = 0;
  
  int tValid1 = 0; //these track how long arm has been in valid range
  int tValidb = 0;
  int tThreshold = 20;
  int initState = state;
  
  while(((tValid1 < tThreshold && value1 != -1) || (tValidb < tThreshold && valueb != -1)) && t2 - tInitial < ARM_TIME_LIMIT && state == initState){
    dt = t2 - t1;
    errInt1 = errInt1 + error1;
    errIntb = errIntb + errorb;
    
    if(errInt1 > 10000) errInt1 = 10000; else if(errInt1 < -10000) errInt1 = -10000;
    if(errIntb > 10000) errIntb = 10000; else if(errIntb < -10000) errIntb = -10000;
    if(valueb != -1){
      int motSpb = ((int32_t)PArm.Value * errorb / 8 - (int32_t)DArm.Value * (errorb - prevErrb)/dt + (int32_t)IArm.Value * errIntb / 1024) / 256;
      
      if(motSpb < ARM_PID_MIN_B && motSpb > ARM_PID_STOP){
        motSpb = ARM_PID_MIN_B; //clamp motor to minimum speed to produce movement
      } else if(motSpb > -ARM_PID_MIN_B && motSpb < -ARM_PID_STOP){
        motSpb = -ARM_PID_MIN_B;
      } else if(motSpb < ARM_PID_STOP && motSpb > -ARM_PID_STOP){
        motSpb = 0;
      }
      
      if(motSpb > 255){
        motSpb = 255;
      } else if(motSpb < -255){
        motSpb = -255;
      }
      
      if(abs(errorb) <= ARM_ERROR){
        tValidb = tValidb + 1; //if arm within arm error for long enough, joint considered to be placed
        motSpb = 0;
      } else {
        tValidb = 0;
      }

      motor.speed(ARM_BASE, motSpb);
      prevErrb = errorb;
    }
    if(value1 != -1){
      int motSp1 = ((int32_t)PArm1.Value * error1 / 64 - (int32_t)DArm1.Value * (error1 - prevErr1)/dt + (int32_t)IArm1.Value * errInt1 / 1024);
      
      if(motSp1 < ARM_PID_MIN_1 && motSp1 > ARM_PID_STOP){
        motSp1 = ARM_PID_MIN_1;
      }
      if(motSp1 > -ARM_PID_MIN_1 && motSp1 < -ARM_PID_STOP){
        motSp1 = -ARM_PID_MIN_1;
      }
      
      if(motSp1 > 255){
        motSp1 = 255;
      } else if(motSp1 < -255){
        motSp1 = -255;
      }
      
      if(abs(error1) <= ARM_ERROR){
        tValid1 = tValid1 + 1;
        motSp1 = 0;
      } else {
        tValid1 = 0;
      }
      
      motor.speed(ARM_1, motSp1);
      prevErr1 = error1;
    }
    t1 = t2;
    t2 = millis();
    errorb = analogRead(ARM_POT_BASE) - valueb;
    error1 = analogRead(ARM_POT_1) - value1;
  }
  
  motor.speed(ARM_POT_BASE, 0);
  motor.speed(ARM_1, 0);
}

void swingL(){
  moveArm(baseAngle + 60, -1, -1);
}
void swingR(){
  moveArm(baseAngle - 60, -1, -1);
}
void raise(){
  moveArm(-1, -1, joint2Angle + 10);
}
void lower(){
  moveArm(-1, -1, joint2Angle - 10);
}
void releasePet(){
  int ti = millis();
  while(digitalRead(ARM_END) == 0 && millis() - ti < 3000){
    ARM_RELEASE.write(180);
    delay(100);
    ARM_RELEASE.write(0);
  }
}

void petPickup(int petNum){
  static int beforeLoc[][20][3] = {
                              {{200, 300, 0}, {400, 600, 180}}
                            };
  static int afterLoc[][20][3] = {
                             {{400, 500, 60}, {200, 500, 180}}
                            };
  
  int beforeLocCurr[20][3];
  memcpy(beforeLocCurr, &beforeLoc[petNum - 1], 2*sizeof(*beforeLoc[petNum - 1]));
  int afterLocCurr[20][3];
  memcpy(afterLocCurr, &afterLoc[petNum - 1], 2*sizeof(*afterLoc[petNum - 1]));
  
  for(int i = 0; i < sizeof(beforeLocCurr) / sizeof(beforeLocCurr[0]); i++){
    if(state == PET_DROPOFF) break;
    moveArm(beforeLocCurr[i][0], beforeLocCurr[i][1], beforeLocCurr[i][2]);
  }
  
  for(int i = 0; i < 5; i++){
    if(state == PET_DROPOFF) break;
    if(i%2 == 0){
      for(int j = 0; j <= 5; j++){
        swingL();
        delay(200);
        if(state == PET_DROPOFF) break;
      }
    } else {
      for(int j = 0; j <= 5; j++){
        swingR();
        delay(200);
        if(state == PET_DROPOFF) break;
      }
    }
  }
  
  if(state == PET_DROPOFF){
    for(int i = 0; i < sizeof(afterLocCurr) / sizeof(afterLocCurr[0]); i++){
      moveArm(afterLocCurr[i][0], afterLocCurr[i][1], afterLocCurr[i][2]);
    }
    releasePet();
  }
  switch(petNum){
    case 1:
    case 2:
    case 3:
      state = TAPE_FOLLOW;
      break;
    case 4:
    case 5:
      state = IR_FOLLOW;
      break;
    case 6:
      state = ZIPLINE;
      break;
  }
  petNum++;
}
