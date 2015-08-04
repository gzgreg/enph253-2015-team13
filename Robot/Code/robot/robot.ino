#include "constants.h"
#include "arm.h"
#include "tape.h"
#include "ir.h"
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
  moveArm(90, 711, 177); //initial arm position
  selectMode();
}
void loop()
{
  if(mode == 1){ //marking detection
    LCD.clear(); LCD.home(); LCD.print(state);
    switch(state){
      case TAPE_FOLLOW_UP:
      case TAPE_FOLLOW_DOWN:
        tapeFollow();
        break;
      case PET_PICKUP: 
        petPickup(petNum);
        break;
      case IR_FOLLOW_F:
      case IR_FOLLOW_B:
        irFollow();
        break;
      case TAPE_SEARCH:
        tapeSearch();
        break;
      case TURN_AROUND:
        encodedMotion(false, 4, false, 4);
        encodedMotion(true, 8, false, 8);
        state = IR_FOLLOW_B;
        break;
    }
  } else if(mode == 2){ //arm movement
    moveArm(-1, map(knob(6), 0, 1024, 200, 800), map(knob(7), 0, 1024, 0, 180));
    LCD.clear(); LCD.home();
    char buffer[1024];
    sprintf(buffer, "%d %d %d", analogRead(ARM_POT_BASE), analogRead(ARM_POT_1) , joint2Angle);
    LCD.print(buffer);    
  } else if(mode == 3){ //IR follow
    state = IR_FOLLOW_F;
    irFollow();
  } else if(mode == 4){
    state = PET_PICKUP;
    petPickup(petNum);
    delay(1000);
  } else if(mode == 5){
    tapeSearch();
  } else if(mode == 6){
    switch(state){
      case TAPE_FOLLOW_UP:
        tapeFollow();
        break;
      case PET_PICKUP: {
        delay(1000);
        switch(petNum){
          case 1:
            encodedMotion(false, 4, false, 4);
            break;
          case 3:
            break;
          case 4:
          case 5:
            encodedMotion(false, 4, false, 4);
            break;
          case 6:
            break;
        }
        if(petNum == 4){
          encodedMotion(true, 10, true, 10);
          encodedMotion(true, 5, false, 0);
          encodedMotion(true, 10, true, 10);
        }
        petNum++;
        selectMode();
        break;
      }
      case CATAPULT:
        fireCatapult();
        break;
      case TAPE_SEARCH:
        tapeSearch();
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
      selectMode();
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

void selectMode(){
  int newMode;
  while(!startbutton()){
    LCD.clear(); LCD.home();
    newMode = map(knob(6), 0, 1024, 1, 8);
    LCD.print(newMode);
    delay(50);
  }
  mode = newMode;
  if(mode == 1) state = TAPE_FOLLOW_UP;
  if(mode == 6){
    int newPetNum;
    while(!stopbutton()){
      LCD.clear(); LCD.home();
      newPetNum = map(knob(6), 0, 1024, 1, 7);
      LCD.print(newPetNum); 
      delay(50);
    }
    petNum = newPetNum;
    state = TAPE_SEARCH;
  }
}
