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
  if(joint2 != -1){
    ARM_2.write(joint2);
    if(base == -1 && joint1 == -1) delay(500);
  }
  armPID(ARM_BASE, base, ARM_1, joint1);
  if(base != -1) baseAngle = analogRead(ARM_POT_BASE);
  if(joint1 != -1) joint1Angle = analogRead(ARM_POT_1);
  if(joint2 != -1){
    joint2Angle = joint2;
  }
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
      int motSp1 = ((int32_t)PArm1.Value * error1 / 1024 - (int32_t)DArm1.Value * (error1 - prevErr1)/dt + (int32_t)IArm1.Value * errInt1 / 1024);
      
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
  int newAngle;
  if(joint2Angle < 60) newAngle = 0; else newAngle = joint2Angle - 60;
  ARM_2.write(newAngle);
  joint2Angle = newAngle;
  delay(500);
}
void lower(){
  int newAngle;
  if(joint2Angle > 120) newAngle = 180; else newAngle = joint2Angle + 60;
  ARM_2.write(newAngle);
  joint2Angle = newAngle;
  delay(500);
}
void releasePet(){
  ARM_RELEASE.write(0);
  delay(1000);
  ARM_RELEASE.write(180);
}

void petPickup(int petNum){
  /* 
    Basket: 483 182 0, -1 -1 180, 800 -1 150, -1 -1 100
    Pet 1:183 625 55
    Pet 2: same
  */
  static int beforeLoc[][2][3] = {
                              {{166, 783, 0}, {-1, -1, 91}},
                              {{148, 820, 0}, {-1, -1, 62}},
                              {{318, 623, 172}, {-1, 724, -1}},
                              {{287, 945, 108}, {-1, -1, -1}},
                              {{-1, -1, -1}, {-1, -1, -1}},
                              {{-1, -1, -1}, {-1, -1, -1}}                              
                            };
  static int afterLoc[][4][3] = {
                             {{445, 303, 180}, {-1, -1, -0}, {800, 300, 0}, {-1, -1, 95}},
                             {{445, 303, 180}, {-1, -1, -0}, {800, 300, 0}, {-1, -1, 95}},
                             {{445, 303, 180}, {-1, -1, -0}, {800, 300, 0}, {-1, -1, 95}},
                             {{445, 303, 180}, {-1, -1, -0}, {800, 300, 0}, {-1, -1, 95}},
                             {{445, 303, 180}, {-1, -1, -0}, {800, 300, 0}, {-1, -1, 95}},
                             {{445, 303, 180}, {-1, -1, -0}, {800, 300, 0}, {-1, -1, 95}}              
                            };
  int n = petNum - 1;
  LCD.clear(); LCD.home(); LCD.print("Picking up pet");delay(50);
  
  for(int i = 0; i < 2; i++){
    if(state == PET_DROPOFF) break;
    char buffer[1024]; sprintf(buffer, "Moving to %d", i); LCD.clear(); LCD.home(); LCD.print(buffer); delay(50);
    moveArm(beforeLoc[n][i][0], beforeLoc[n][i][1], beforeLoc[n][i][2]);
  }
  LCD.clear(); LCD.home(); LCD.print("Searching"); delay(50);
  for(int i = 0; i < 5; i++){
    if(state == PET_DROPOFF) break;
    if(i%2 == 0){
      int temp;
      if(i == 0) temp = 25; else temp = 50;
      raise();
      swingL(temp);
      lower();
      if(state == PET_DROPOFF) break;
    } else {
      raise();
      swingR(50);
      lower();
      if(state == PET_DROPOFF) break;
    }
  }
  LCD.clear(); LCD.home(); LCD.print("Pet found"); delay(50);
  for(int i = 0; i < 4; i++){
    moveArm(afterLoc[n][i][0], afterLoc[n][i][1], afterLoc[n][i][2]);
  }
  releasePet();
  switch(petNum){
    case 1:
      state = TURN_AROUND;
    case 2:
      state = IR_FOLLOW_B;
      break;
    case 3:
    case 4:
    case 5:
      state = TAPE_FOLLOW_DOWN;
      break;
  }
  petNum++;
}
