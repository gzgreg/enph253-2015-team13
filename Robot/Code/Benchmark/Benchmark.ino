#include <phys253.h>
#include <LiquidCrystal.h>

#define RUNS 10000
int i;
unsigned long dt;
unsigned long dtFunc;
unsigned long ti;
unsigned long tf;

void setup()
{
  #include <phys253setup.txt>
  Serial.begin(9600);
  
  i = 0;
  ti = micros();
  while(i < RUNS){
    i++;
  }
  tf = micros();
  dt = tf - ti;
  
  i = 0;
  ti = micros();
  while(i < RUNS){
    sin(20);
    i++;
  }
  tf = micros();
  dtFunc = tf - ti;
  
  LCD.clear();
  LCD.home();
  LCD.print(dt);
  LCD.setCursor(0, 1);
  LCD.print(dtFunc);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
