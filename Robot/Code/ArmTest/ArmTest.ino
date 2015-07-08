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
  moveArm(200, 100, 90);
  delay(1000);
  moveArm(300, -1, -1);
  delay(1000);
  moveArm(-1, 60, 30);
  delay(1000);
}
