#include <LiquidCrystal.h>   
#include <phys253.h>           
#define ANALOG_PIN 0
#define PAUSE_TIME 420


void setup() {

  #include <phys253setup.txt>
  Serial.begin(9600);


}

void loop() {
  
  int val;
  val = analogRead(ANALOG_PIN);
  LCD.print(val);
  delay(PAUSE_TIME);
  LCD.clear();
}
