#include "constants.h"
#include "arm.h"
#include "tape.h"
#include "ir.h"
#include "interrupts.h"
#include "catapult.h"
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
  
//  enableExternalInterrupt(INT0, RISING);
//  enableExternalInterrupt(INT1, RISING);
  //enableExternalInterrupt(INT2, FALLING);
  
  ARM_RELEASE.write(180);
  BASKET_RELEASE.write(180);
  //moveArm(78, 900, 0); //initial arm position
  int newMode;
  while(!startbutton()){
    LCD.clear(); LCD.home();
    newMode = map(knob(6), 0, 1024, 1, 8);
    LCD.print(newMode);
    delay(50);
  }
  mode = newMode;
  state = TAPE_FOLLOW;
}
int j = 0; //temp
void loop()
{
  if(mode == 1){ //marking detection
    switch(state){
      case TAPE_FOLLOW:
        tapeFollow();
        j=0;
        break;
      case PET_PICKUP: {
        delay(1000);
        switch(petNum){
          case 1:
          case 2:
            state = TAPE_FOLLOW;
            encodedMotion(false, 4, false, 4);
            break;
          case 3:
            state = CATAPULT;
            break;
          case 4:
          case 5:
            state = IR_FOLLOW;
            encodedMotion(false, 4, false, 4);
            break;
          case 6:
            state = ZIPLINE;
            break;
        }
        if(petNum == 4){
          encodedMotion(true, 10, true, 10);
          encodedMotion(true, 5, false, 0);
          encodedMotion(true, 10, true, 10);
        }
        petNum++;
        break;
      }
      case IR_FOLLOW:
        irFollow();
        j = 0;
        break;
      case CATAPULT:
        fireCatapult();
        break;
    }
  } else if(mode == 2){ //arm movement
    moveArm(-1, map(knob(6), 0, 1024, 400, 800), map(knob(7), 0, 1024, 0, 180));
    LCD.clear(); LCD.home();
    char buffer[1024];
    sprintf(buffer, "%d %d %d", analogRead(ARM_POT_BASE), analogRead(ARM_POT_1) , joint2Angle);
    LCD.print(buffer);    
  } else if(mode == 3){ //IR follow
    state = IR_FOLLOW;
    irFollow();
  } else if(mode == 4){
    state = PET_PICKUP;
    petPickup(petNum);
    delay(1000);
  } else if(mode == 5){
    LCD.clear(); LCD.home(); LCD.print(analogRead(ARM_POT_1));
    delay(50);
  } else if(mode == 6){
    switch(state){
      case TAPE_FOLLOW:
        tapeFollow();
        break;
      case PET_PICKUP: {
        LCD.clear(); LCD.home(); LCD.print(petNum);
        encodedMotion(false, 4, false, 4);
        petPickup(petNum);
        break;
      }
      case CATAPULT:
        fireCatapult();
        break;
      default:
        LCD.clear(); LCD.home(); LCD.print(state);
    }
  } else if(mode == 7){
    state = CATAPULT;
    fireCatapult();
  }
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
