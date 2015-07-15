//Include this file in all robot program folders.

#include <phys253.h>
#include <LiquidCrystal.h>

#define LEFT_SENSOR 0
#define RIGHT_SENSOR 1
#define ARM_POT_BASE 2
#define ARM_POT_1 3
#define LEFT_IR 4
#define RIGHT_IR 5
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 0
#define ARM_BASE 2
#define ARM_1 3
#define ARM_2 RCServo1
#define ARM_ERROR 20
#define ARM_PID_STOP 10
#define ARM_PID_MIN 50
#define ARM_TIME_LIMIT 3000
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
MenuItem PArm = MenuItem("PArm", 1023);
MenuItem DArm = MenuItem("DArm", 1023);
MenuItem menuItems[] = {Speed, PTape, DTape, Thresh, PIR, DIR, PArm, DArm};
 
void Menu(){
  LCD.clear(); LCD.home();
  LCD.print("Entering menu");
  delay(500);
 
  while (true){
    /* Show MenuItem value and knob value */
    int menuIndex = knob(6) * (MenuItem::MenuItemCount) / 1024;
    LCD.clear(); LCD.home();
    LCD.print(menuItems[menuIndex].Name); LCD.print(" "); LCD.print(menuItems[menuIndex].Value);
    LCD.setCursor(0, 1);
    LCD.print("Set to "); LCD.print(map(knob(7), 0, 1024, 0, menuItems[menuIndex].Max + 1)); LCD.print("?");
    delay(100);
 
    /* Press start button to save the new value */
    if (startbutton()){
      delay(50);
      if (startbutton()){
        menuItems[menuIndex].Value = map(knob(7), 0, 1024, 0, menuItems[menuIndex].Max + 1);
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

void armPID(int motor1, int value1, int motor2, int value2){
  int error1 = analogRead(ARM_POT_BASE) - value1;
  int error2 = analogRead(ARM_POT_1) - value2;
  
  int prevErr1 = error1;
  int prevErr2 = error2;
  unsigned long t1 = millis();
  unsigned long t2 = t1;
  unsigned long tInitial = t1;
  int dt = 0;
  
  if(error1 > ARM_ERROR){
    motor.speed(ARM_BASE, 255);
  } else if(error1 < -ARM_ERROR){
    motor.speed(ARM_BASE, -255);
  }
  if(error2 > ARM_ERROR){
    motor.speed(ARM_1, 255);
  } else if(error2 < -ARM_ERROR){
    motor.speed(ARM_1, -255);
  }
  delay(5);
  while((abs(error1) > ARM_ERROR || value1 == -1) && (abs(error2) > ARM_ERROR || value2 == -1) && t2 - tInitial < ARM_TIME_LIMIT){
    dt = t2 - t1;
    if(abs(error1) > ARM_ERROR && value1 != -1){
      int motSp1 = (PArm.Value * error1 - DArm.Value * (error1 - prevErr1)/dt);
      
      if(motSp1 < ARM_PID_MIN && motSp1 > ARM_PID_STOP){
        motSp1 = ARM_PID_MIN;
      }
      if(motSp1 > -ARM_PID_MIN && motSp1 < -ARM_PID_STOP){
        motSp1 = -ARM_PID_MIN;
      }
      motor.speed(ARM_BASE, motSp1);
      prevErr1 = error1;
    }
    if(abs(error2) > ARM_ERROR && value2 != -1){
      int motSp2 = (PArm.Value * error2 - DArm.Value * (error2 - prevErr2)/dt);
      
      if(motSp2 < ARM_PID_MIN && motSp2 > ARM_PID_STOP){
        motSp2 = ARM_PID_MIN;
      }
      if(motSp2 > -ARM_PID_MIN && motSp2 < -ARM_PID_STOP){
        motSp2 = -ARM_PID_MIN;
      }
      motor.speed(ARM_1, motSp2);
      prevErr2 = error2;
    }
    t1 = t2;
    t2 = millis();
  }
}
