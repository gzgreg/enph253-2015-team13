#include "constants.h"

void moveArm(int, int, int);
void armPID(int, int, int, int);
void swingL(int);
void swingR(int);
void raise(int);
void lower(int);
void releasePet();
void petPickup(int);

/*
Putting -1 into any parameter will cause that piece of the arm to stay stationary.
base and joint1 take pot values, not angles. joint2 takes an angle. These should be found empirically
*/
void moveArm(int base, int joint1, int joint2) {
  if (joint2 != -1) {
    ARM_2.write(joint2);
    if (base == -1 && joint1 == -1) delay(500);
  }
  armPID(ARM_BASE, base, ARM_1, joint1);
  if (base != -1) baseAngle = analogRead(ARM_POT_BASE);
  if (joint1 != -1) joint1Angle = analogRead(ARM_POT_1);
  if (joint2 != -1) {
    joint2Angle = joint2;
  }
  motor.stop_all();
}

void armPID(int motorb, int valueb, int motor1, int value1) {
  int error1 = analogRead(ARM_POT_1) - value1;
  int errorb = analogRead(ARM_POT_BASE) - valueb;

  int prevErr1 = error1;
  int prevErrb = errorb;
  int errIntb = error1;
  int errInt1 = errorb;
  unsigned long t1 = millis();
  unsigned long t2 = t1 + 1000;
  unsigned long tInitial = t1;
  int dt = 0;

  int tValid1 = 0; //these track how long arm has been in valid range
  int tValidb = 0;
  int tThreshold = 20;

  while (((tValid1 < tThreshold && value1 != -1) || (tValidb < tThreshold && valueb != -1)) && t2 - tInitial < ARM_TIME_LIMIT) {
    if (!digitalRead(ARM_END) && state == PET_PICKUP && petNum != 1) {
      delay(50);
      raise(40);
      if (!digitalRead(ARM_END)) {
        state = PET_DROPOFF;
        motor.stop_all();
        break;
      }
    }
    dt = t2 - t1;
    errInt1 = errInt1 + error1;
    errIntb = errIntb + errorb;

    if (errInt1 > 10000) errInt1 = 10000; else if (errInt1 < -10000) errInt1 = -10000;
    if (errIntb > 10000) errIntb = 10000; else if (errIntb < -10000) errIntb = -10000;
    if (valueb != -1) {
      int motSpb = ((int32_t)PArm.Value * errorb / 8 - (int32_t)DArm.Value * (errorb - prevErrb) / dt + (int32_t)IArm.Value * errIntb / 1024) / 256;

      if (motSpb < ARM_PID_MIN_B && motSpb > ARM_PID_STOP) {
        motSpb = ARM_PID_MIN_B; //clamp motor to minimum speed to produce movement
      } else if (motSpb > -ARM_PID_MIN_B && motSpb < -ARM_PID_STOP) {
        motSpb = -ARM_PID_MIN_B;
      } else if (motSpb < ARM_PID_STOP && motSpb > -ARM_PID_STOP) {
        motSpb = 0;
      }

      if (motSpb > 255) {
        motSpb = 255;
      } else if (motSpb < -255) {
        motSpb = -255;
      }

      if (abs(errorb) <= ARM_ERROR) {
        tValidb = tValidb + 1; //if arm within arm error for long enough, joint considered to be placed
        motSpb = 0;
      } else {
        tValidb = 0;
      }

      motor.speed(ARM_BASE, motSpb);
      prevErrb = errorb;
    }
    if (value1 != -1) {
      int motSp1 = ((int32_t)PArm1.Value * error1 / 1024 - (int32_t)DArm1.Value * (error1 - prevErr1) / dt + (int32_t)IArm1.Value * errInt1 / 1024);

      if (motSp1 < ARM_PID_MIN_1 && motSp1 > ARM_PID_STOP) {
        motSp1 = ARM_PID_MIN_1;
      }
      if (motSp1 > -ARM_PID_MIN_1 && motSp1 < -ARM_PID_STOP) {
        motSp1 = -ARM_PID_MIN_1;
      }

      if (motSp1 > 255) {
        motSp1 = 255;
      } else if (motSp1 < -255) {
        motSp1 = -255;
      }

      if (abs(error1) <= ARM_ERROR) {
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

  motor.speed(ARM_BASE, 0);
  motor.speed(ARM_1, 0);
}

void swingL(int dist) {
  moveArm(baseAngle + dist, -1, -1);
}
void swingR(int dist) {
  moveArm(baseAngle - dist, -1, -1);
}
void raise(int angle) {
  int newAngle;
  if (joint2Angle < angle) newAngle = 0; else newAngle = joint2Angle - angle;
  moveArm(-1, -1, newAngle);
}
void lower(int angle) {
  int newAngle;
  if (joint1Angle > 1000 - angle) newAngle = 1000; else newAngle = joint1Angle + angle;
  moveArm(-1, newAngle, -1);
}
void releasePet() {
  ARM_RELEASE.write(75);
  delay(1000);
  ARM_RELEASE.write(0);
}

void petPickup(int petNumber) {
  /*
    Basket: 483 182 0, -1 -1 180, 800 -1 150, -1 -1 100
    Pet 1:183 625 55
    Pet 2: same
  */
  if(petNum >= 5) return;
  static int beforeLoc[][2][3] = {
    {{ -1, -1, -1}, { -1, -1, -1}}, //pet 1
    {{-1, 485, 49}, {300, 628, 98}}, //pet 2
    {{578, 709, 68}, {578, 709, 68}}, //pet 3
    {{680, 685, 67}, {680, 685, 67}}, //pet 4
    {{ -1, -1, -1}, { -1, -1, -1}}, // pet 5
    {{ -1, -1, -1}, { -1, -1, -1}} //pet 6
  };
  static int afterLoc[][3][3] = {
    {{-1, -1, 60}, {346, 259, 0}, { 40, -1, -1}},
    {{-1, 346, 100}, {53, -1, 0}, {-1, -1, -1}},
    {{500, 259, 100}, {53, -1, 0}, {-1, -1, -1}},
    {{500, 259, 100}, {876, 543, 52}, {-1, -1, -1}},
    {{500, 259, 100}, {127, -1, 52}, {-1, -1, -1}},
    {{500, 259, 100}, {127, -1, 52}, {-1, -1, -1}}
  };
  int n = petNumber - 1;
  LCD.clear(); LCD.home(); char buffer[1024]; sprintf(buffer, "Picking up pet %d", petNumber); LCD.print(buffer); delay(1000);

  for (int i = 0; i < 2; i++) {
    if (state == PET_DROPOFF) break;
    char buffer[1024]; sprintf(buffer, "Moving to %d", i); LCD.clear(); LCD.home(); LCD.print(buffer); delay(50);
    moveArm(beforeLoc[n][i][0], beforeLoc[n][i][1], beforeLoc[n][i][2]);
  }
  LCD.clear(); LCD.home(); LCD.print("Searching"); delay(200);
  for (int i = 0; i < 5; i++) {
    if (state == PET_DROPOFF) break;
    if (i % 2 == 0) {
      int temp;
      if (i == 0) temp = 25; else temp = 50;
      swingL(temp);
      delay(500);
      if (state == PET_DROPOFF) break;
    } else {
      swingR(50);
      delay(500);
      if (state == PET_DROPOFF) break;
      lower(30);
      delay(200);
    }
    
//    moveArm(beforeLoc[n][2][0], beforeLoc[n][2][1], beforeLoc[n][2][2]);
//    if(i % 2 == 0){
//      swingL(i / 2 * 25);
//    } else {
//      swingR(i / 2 * 25);
//    }
//    lower(100);
  }
  LCD.clear(); LCD.home(); LCD.print("Pet found"); delay(50);
  for (int i = 0; i < 3; i++) {
    moveArm(afterLoc[n][i][0], afterLoc[n][i][1], afterLoc[n][i][2]);
  }
  if(petNum != 4){
    releasePet();
  } else {
    
  }
  switch (petNum) {
    case 1:
    case 2:
    case 3:
    case 4:
      state = TAPE_SEARCH;
      break;
  }
  petNum = petNum + 1;
  
  if(petNum == 5){
    moveArm(876, 850, 43);
  }
}
