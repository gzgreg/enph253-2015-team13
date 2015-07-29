#include "constants.h"
#include "arm.h"
#include "tape.h"
#include "ir.h"
#include "interrupts.h"
#include <phys253.h>
#include <LiquidCrystal.h>

int mode;

void setup()
{
  #include <phys253setup.txt>
  Serial.begin(9600);
  mode = 0;
  petNum = 1;
  leftRotations = 0;
  rightRotations = 0;
  onMarking = false;
  
  //enableExternalInterrupt(INT0, RISING);
  //enableExternalInterrupt(INT1, RISING);
  //enableExternalInterrupt(INT2, FALLING);
  
  ARM_RELEASE.write(180);
  int newMode;
  while(!startbutton()){
    LCD.clear(); LCD.home();
    newMode = map(knob(6), 0, 1024, 1, 8);
    LCD.print(newMode);
    delay(50);
  }
  mode = newMode;
  //moveArm(78, 900, 0); //initial arm position
  state = TAPE_FOLLOW;
}
int j = 0; //temp
void loop()
{
//  if(mode == 1){ //marking detection
//    switch(state){
//      case TAPE_FOLLOW:
//        tapeFollow();
//        j=0;
//        break;
//      case PET_PICKUP:
//        motor.stop_all();
//        delay(1000);
//        int currRotLeft = leftRotations;
//        int currRotRight = rightRotations;
//        while(leftRotations - currRotLeft < 4 && rightRotations - currRotRight < 4 && j == 0){
//          if(leftRotations - currRotLeft < 4) motor.speed(LEFT_MOTOR, -150);
//          if(rightRotations - currRotRight < 4) motor.speed(RIGHT_MOTOR, 150);
//        }
//        motor.stop_all();
//        LCD.clear(); LCD.home();
//        LCD.print(leftRotations);
//        LCD.setCursor(0, 1);
//        LCD.print(rightRotations);
//        j++;
//        
//        break;
//    }
//  } else if(mode == 2){ //arm movement
//    moveArm(-1, knob(6), map(knob(7), 0, 1024, 0, 180));
//    LCD.clear(); LCD.home();
//    char buffer[1024];
//    sprintf(buffer, "%d %d %d", baseAngle, joint1Angle, joint2Angle);
//    LCD.print(buffer);    
//  } else if(mode == 3){ //IR follow
//    state = IR_FOLLOW;
//    irFollow();
//  } else if(mode == 4){
    state = PET_PICKUP;
    petPickup(petNum);
    delay(1000);
//  } else if(mode == 5){
//    LCD.clear(); LCD.home(); LCD.print(digitalRead(ARM_END));
//    delay(50);
//  } else if(mode == 6){
//    motor.speed(LEFT_MOTOR, 150);
//    motor.speed(RIGHT_MOTOR, 150);
//    LCD.clear(); LCD.home(); LCD.print(leftRotations); LCD.setCursor(0, 1); LCD.print(rightRotations);
//  } else if(mode == 7){
//    LCD.clear(); LCD.home();
//    char buffer[1024];
//    sprintf(buffer, "%d %d %d %d", analogRead(L_MARK_SENSOR), analogRead(LEFT_SENSOR), analogRead(RIGHT_SENSOR), analogRead(R_MARK_SENSOR));
//    LCD.print(buffer);
//    delay(50);
//  }
  if(stopbutton()){
    delay(50);
    if(stopbutton()){
      int newMode;
      while(!startbutton()){
        LCD.clear(); LCD.home();
        newMode = map(knob(6), 0, 1024, 1, 8);
        LCD.print(newMode);
        delay(50);
      }
      mode = newMode;
      if(mode == 1) state = TAPE_FOLLOW;
    }
  }
  
  if(startbutton()){
    delay(50);
    if(startbutton()){
      motor.stop_all();
      Menu();
    }
  }
}
