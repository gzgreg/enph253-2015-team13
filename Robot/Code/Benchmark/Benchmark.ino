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
<<<<<<< HEAD
  #include <phys253setup.txt>
  Serial.begin(9600);
=======
//  #include <phys253setup.txt>
//  Serial.begin(9600);
>>>>>>> 712cab93ef562321daa3bfa0842639beac2d5744
//  
//  i = 0;
//  ti = micros();
//  while(i < RUNS){
//    i++;
//  }
//  tf = micros();
//  dt = tf - ti;
//  
//  i = 0;
//  ti = micros();
//  while(i < RUNS){
//    sin(20);
//    i++;
//  }
//  tf = micros();
//  dtFunc = tf - ti;
//  
//  LCD.clear();
//  LCD.home();
//  LCD.print(dt);
//  LCD.setCursor(0, 1);
<<<<<<< HEAD
//  LCD.print(dtFunc);
//  
=======
//  LCD.print(dtFunc);  
>>>>>>> 712cab93ef562321daa3bfa0842639beac2d5744
}

void loop() {
  // put your main code here, to run repeatedly:
<<<<<<< HEAD
  motor.speed(0, knob(6));
=======
  LCD.clear(); LCD.home(); LCD.print(stopbutton());
  delay(50);
>>>>>>> 712cab93ef562321daa3bfa0842639beac2d5744
}
