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
}

void loop()
{
  if(mode == 1){ //marking detection
    switch(state){
      case TAPE_FOLLOW:
        tapeFollow();
        break;
      case PET_PICKUP:
        LCD.clear(); LCD.home();
        LCD.print("Markings detected.");
        delay(50);
        break;
    }
  } else if(mode == 2){ //arm movement
    moveArm(-1, knob(6), map(knob(7), 0, 1024, 0, 180));
  } else if(mode == 3){ //IR follow
    irFollow();
  }
  
  if(stopbutton()){
    delay(50);
    if(stopbutton()){
      int newMode;
      while(!startbutton()){
        LCD.clear(); LCD.home();
        newMode = map(knob(6), 0, 1024, 1, 4);
        LCD.print(newMode);
        delay(50);
      }
      mode = newMode;
    }
  }
}
