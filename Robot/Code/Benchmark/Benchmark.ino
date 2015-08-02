#include <phys253.h>
#include <LiquidCrystal.h>

#define RUNS 10000
int i=0;
unsigned long dt;
unsigned long dtFunc;
unsigned long ti;
unsigned long tf;
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1

void setup()
{

  #include <phys253setup.txt>
  Serial.begin(9600);
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
//  LCD.print(dtFunc);  
}
void loop() {
  // put your main code here, to run repeatedly:
  
  char buffer[1024];
  sprintf(buffer, "%d %d %d %d", analogRead(0), analogRead(1), analogRead(2), analogRead(3));
  LCD.clear(); LCD.home(); LCD.print(buffer);
  LCD.setCursor(0, 1);
  sprintf(buffer, "%d %d %d %d", analogRead(4), analogRead(5), analogRead(6), analogRead(7));
  LCD.print(buffer);
  delay(50);

//  encodedMotion(true, 25, true, 25);
//  encodedMotion(false, 25, false, 25);

//  if(stopbutton()){
//    delay(10);
//    if(stopbutton()){
//      LCD.clear(); LCD.home(); LCD.print("Switching");
//      delay(300);
//      while(!stopbutton()){
//        LCD.clear(); LCD.home();
//        LCD.print(digitalRead(0));
//        LCD.setCursor(0, 1);
//        LCD.print(digitalRead(1));
//        delay(50);
//      }
//    }
//  }

//    int dig0 = digitalRead(0);
//    int dig1 = digitalRead(1);
//    LCD.clear(); LCD.home();
//    char buffer[1024];
//    sprintf(buffer, "%lu %lu %d %d %d", leftRotations, rightRotations, dig0, dig1, i);
//    LCD.print(buffer);
//    delay(50);

//  RCServo0.write(map(knob(6), 0, 1024, 0, 181));
}

void encodedMotion(bool lForward, int lRot, bool rForward, int rRot){  
  motor.stop_all();
  delay(500);
  int leftRotations = 0;
  int rightRotations = 0;
  int lSpeed; if(lForward) lSpeed = 100; else lSpeed = -100;
  int rSpeed; if(rForward) rSpeed = -100; else rSpeed = 100;
  bool lOn = digitalRead(0);
  bool rOn = digitalRead(1);
  motor.speed(LEFT_MOTOR, lSpeed);
  motor.speed(RIGHT_MOTOR, rSpeed);
  while(leftRotations < lRot || rightRotations < rRot){
    if(digitalRead(0) && !lOn){
      delay(15);
      if(digitalRead(0)){
        leftRotations++;
        if(leftRotations >= lRot) motor.speed(LEFT_MOTOR, 0);
      }
    } else if(!digitalRead(0)){
      delay(15);
      if(!digitalRead(0)){
        lOn = false;
      }
    }
    
    if(digitalRead(1) && !rOn){
      delay(15);
      if(digitalRead(1)){
        rightRotations++;
        if(rightRotations >= rRot) motor.speed(RIGHT_MOTOR, 0);
      }
    } else if(!digitalRead(1)){
      delay(15);
      if(!digitalRead(1)){
        rOn = false;
      }
    }
    LCD.clear(); LCD.home();
    char buffer[1024];
    sprintf(buffer, "%d %d %d %d %d", leftRotations, rightRotations, digitalRead(0), digitalRead(1), i);
    LCD.print(buffer);
    delay(10);
  }
}
