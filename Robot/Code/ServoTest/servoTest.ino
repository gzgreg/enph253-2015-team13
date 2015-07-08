#include <phys253.h>
#include <LiquidCrystal.h>

bool pos = false;

void setup()
{
  #include <phys253setup.txt>
  Serial.begin(9600);
}

void loop(){
  if(startbutton()){
    delay(50);
    if(startbutton()){
      if(pos) RCServo1.write(180); else RCServo1.write(0);
      pos = !pos;
    }
  }
}
