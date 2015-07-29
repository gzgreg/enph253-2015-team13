//Include this file in all robot program folders.

#include <phys253.h>
#include <LiquidCrystal.h>

#define LEFT_SENSOR 0
#define RIGHT_SENSOR 1
#define ARM_POT_BASE 2
#define ARM_POT_1 3
#define LEFT_IR 4
#define RIGHT_IR 5
#define L_MARK_SENSOR 6
#define R_MARK_SENSOR 7
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 0
#define ARM_BASE 2
#define ARM_1 3
#define ARM_2 RCServo1
#define ARM_ERROR 30
#define ARM_PID_STOP 10 //value of arm motors that stops the motors
#define ARM_PID_MIN_B 100 //minimum value of arm base motor
#define ARM_PID_MIN_1 100 //minimum value of arm joint 1 motor
#define ARM_TIME_LIMIT 5000 //time for arm to get into position
#define BUTTON_WAIT 200

#include <avr/EEPROM.h>
 
class MenuItem{
public:
  String    Name;
  int Value;
  uint16_t* EEPROMAddress;
  int Max;
  static uint16_t MenuItemCount;
  MenuItem(String name, int max){
    MenuItemCount++;
    EEPROMAddress = (uint16_t*)(2 * MenuItemCount);
    Name = name;
    Value = (int) eeprom_read_word(EEPROMAddress);
    Max = max;
  }
  void Save(){
    eeprom_write_word(EEPROMAddress, Value);
  }
};
 
uint16_t MenuItem::MenuItemCount = 0;
/* Add the menu items here */
MenuItem Speed = MenuItem("Speed", 255);
MenuItem PTape = MenuItem("P-Tape", 1023);
MenuItem DTape = MenuItem("D-Tape", 1023);
MenuItem Thresh = MenuItem("Thresh-Tape", 1023);
MenuItem PIR = MenuItem("P-IR", 1023);
MenuItem DIR = MenuItem("D-IR", 1023);
MenuItem PArm = MenuItem("PArm-Base", 1023);
MenuItem DArm = MenuItem("DArm-Base", 1023);
MenuItem IArm = MenuItem("IArm-Base", 1023);
MenuItem PArm1 = MenuItem("PArm-1", 1023);
MenuItem DArm1 = MenuItem("DArm-1", 1023);
MenuItem IArm1 = MenuItem("IArm-1", 1023);
MenuItem menuItems[] = {Speed, PTape, DTape, Thresh, PIR, DIR, PArm, DArm, IArm, PArm1, DArm1, IArm1};
 
void Menu(){
  LCD.clear(); LCD.home();
  LCD.print("Entering menu");
  delay(500);
 
  while (true){
    /* Show MenuItem value and knob value */
    int menuIndex = knob(6) * (MenuItem::MenuItemCount) / 1024;
    int val = map(knob(7), 0, 1024, 0, menuItems[menuIndex].Max + 1);
    LCD.clear(); LCD.home();
    LCD.print(menuItems[menuIndex].Name); LCD.print(" "); LCD.print(menuItems[menuIndex].Value);
    LCD.setCursor(0, 1);
    LCD.print("Set to "); LCD.print(val); LCD.print("?");
    delay(100);
 
    /* Press start button to save the new value */
    if (startbutton()){
      delay(50);
      if (startbutton()){
        menuItems[menuIndex].Value = val;
        menuItems[menuIndex].Save();
        delay(250);
      }
    }
 
    /* Press stop button to exit menu */
    if (stopbutton()){
      delay(50);
      if (stopbutton()){
        LCD.clear(); LCD.home();
        LCD.print("Leaving menu");
        delay(500);
        return;
      }
    }
  }
}

void moveArm(int, int, int);
void armPID(int, int, int, int);

int baseAngle;
int joint1Angle;
int joint2Angle;

/*
Putting -1 into any parameter will cause that piece of the arm to stay stationary.
base and joint1 take pot values, not angles. joint2 takes an angle. These should be found empirically
*/
void moveArm(int base, int joint1, int joint2){
  if(joint2 != -1) ARM_2.write(joint2);
  armPID(ARM_BASE, base, ARM_1, joint1);
  baseAngle = base;
  joint1Angle = joint1;
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
  int i = 99; //temp
  
  while(((tValid1 < tThreshold && value1 != -1) || (tValidb < tThreshold && valueb != -1)) && t2 - tInitial < ARM_TIME_LIMIT){
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
      
       //temp
      i = i+1;
      if(i == 100){
        i=0;
        LCD.clear();
        LCD.home();
        char buffer[1024];
        sprintf(buffer, "%d %d %d", value1, analogRead(ARM_POT_1), tValid1);
        LCD.print(buffer);
        LCD.setCursor(0, 1);
        sprintf(buffer, "%d %d %d", valueb, analogRead(ARM_POT_BASE), tValidb);
        LCD.print(buffer);
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
