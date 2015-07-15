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
  moveArm(1000, 1000, 180);
  delay(1000);
  moveArm(100, 100, 0);
  delay(1000);
}
