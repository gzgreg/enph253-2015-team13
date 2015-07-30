#include "constants.h"

void moveArm(int, int, int);
void armPID(int, int, int, int);
void swingL(int);
void swingR(int);
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
      
      char buffer[1024];
      sprintf(buffer, "%d %d %d", motSp1, error1, value1);
      LCD.clear(); LCD.home(); LCD.print(buffer);
      delay(20);
    }
    t1 = t2;
    t2 = millis();
    errorb = analogRead(ARM_POT_BASE) - valueb;
    error1 = analogRead(ARM_POT_1) - value1;
    
    if(!digitalRead(ARM_END) && initState == PET_PICKUP){
      delay(50);
      raise();
      if(!digitalRead(ARM_END)){
        state = PET_DROPOFF;
      }
    }
    
    
  }
  
  motor.speed(ARM_POT_BASE, 0);
  motor.speed(ARM_1, 0);
}

void swingL(int dist){
  moveArm(baseAngle + dist, -1, -1);
}
void swingR(int dist){
  moveArm(baseAngle - dist, -1, -1);
}
void raise(){
  ARM_2.write(joint2Angle+30);
}
void lower(){
  ARM_2.write(joint2Angle - 30);
}
void releasePet(){
  ARM_RELEASE.write(0);
  delay(1000);
  ARM_RELEASE.write(180);
}

void petPickup(int petNum){
  static int beforeLoc[][1][3] = {
                              {{470, 920, 0}},
                              {{470, 920, 0}},
                              {{283, 951, 95}},
                              {{287, 945, 72}}                               
                            };
  static int afterLoc[][2][3] = {
                             {{864, 377, 145}, {870, 582, 145}},
                             {{864, 377, 145}, {870, 582, 145}},
                             {{864, 377, 145}, {870, 582, 145}},
                             {{864, 377, 145}, {870, 582, 145}},                      
                            };
  int n = petNum - 1;
  for(int i = 0; i < 1; i++){
    if(state == PET_DROPOFF) break;
    moveArm(beforeLoc[n][i][0], beforeLoc[n][i][1], beforeLoc[n][i][2]);
  }
  
  for(int i = 0; i < 5; i++){
    if(state == PET_DROPOFF) break;
    if(i%2 == 0){
      int temp;
      if(i == 0) temp = 50; else temp = 100;
      swingL(temp);
      delay(200);
      if(state == PET_DROPOFF) break;
    } else {
      swingR(100);
      delay(200);
      if(state == PET_DROPOFF) break;
    }
  }
  
  if(state == PET_DROPOFF){
    for(int i = 0; i < 2; i++){
      moveArm(afterLoc[n][i][0], afterLoc[n][i][1], afterLoc[n][i][2]);
    }
    releasePet();
  }
  switch(petNum){
    case 1:
    case 2:
      state = TAPE_FOLLOW;
      break;
    case 3:
      state = CATAPULT;
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
