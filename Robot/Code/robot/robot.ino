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
  
  ARM_RELEASE.write(0);
  moveArm(814, 646, 177); //initial arm position
  selectMode();
}
void loop()
{
  if(mode == 1){ //marking detection
    //LCD.clear(); LCD.home(); LCD.print(state);
    switch(state){
      case TAPE_FOLLOW_UP:
      case TAPE_FOLLOW_DOWN:
        tapeFollow();
        break;
      case PET_PICKUP: 
        //petPickup(petNum);
        petNum++;
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
      case TAPE_FOLLOW_DOWN:
        tapeFollow();
        break;
      case PET_PICKUP:
        selectMode();
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
        encodedMotion(true, 16, false, 16);
        state = IR_FOLLOW_B;
        break;
    } 
  } else if(mode == 7){
     releasePet();
     delay(1000);
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
  if(mode == 1){
    state = TAPE_FOLLOW_UP;
    passedMarkings = 0;
  }
  if(mode == 6){
    int newPetNum;
    while(!stopbutton()){
      LCD.clear(); LCD.home();
      newPetNum = map(knob(6), 0, 1024, 1, 7);
      LCD.print(newPetNum); 
      delay(50);
    }
    while(stopbutton()){}
    petNum = newPetNum;
    state = TAPE_SEARCH;
    passedMarkings = 5;
  }
}
