#include "preprocessor.h"
#include <phys253.h>
#include <LiquidCrystal.h>

int errDeriv, prevErr, errTime, prevErrTime, threshold, i;

void setup()
{
  #include <phys253setup.txt>
  Serial.begin(9600);
}

void loop()
{   
  moveArm(400, 300, 180);
  delay(500);
  moveArm(800, 900, 0);
  delay(500);
  //LCD.clear(); LCD.home(); LCD.print("Waiting...");
  //delay(10);
  if(startbutton()){
    delay(50);
    if(startbutton()){
      Menu();
    }
  }

//  int val = map(knob(6), 0, 1024, -255, 256);
//  motor.speed(3, val);
//  LCD.clear(); LCD.home(); LCD.print(val);
//  delay(20);
}
