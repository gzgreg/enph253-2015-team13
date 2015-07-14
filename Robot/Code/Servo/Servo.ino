#include <phys253.h>
#include <LiquidCrystal.h>
#define POTPIN 7
#define MOTPIN 0
int loops = 0;
int prevErr =0;
int P=2,I=0,D=70;
int culErr =0;
int t1,t2,dt;
void setup() {
  // put your setup code here, to run once:
  #include <phys253setup.txt>
  Serial.begin(9600) ;
  LCD.clear();LCD.home();LCD.print("hello World");
  delay(2000);
  LCD.clear();
  
}

void loop() { 
  dt = t1-t2;
   t1=millis();
  loops = (loops+1)%100;
    if(loops==4){LCD.clear();}
 
  
int error = analogRead(POTPIN) - knob(6);

int motSpeed = (P*error + I*culErr - D*(error-prevErr)/dt );
LCD.home();LCD.print(knob(6));
LCD.setCursor(0,2);LCD.print(analogRead(POTPIN)*90.0/1023.0);
LCD.setCursor(6,2);LCD.print(motSpeed);
if(motSpeed<50 && motSpeed>10){motSpeed=50;}
if(motSpeed>-50 && motSpeed<-10){motSpeed=-50;}
motor.speed(MOTPIN,motSpeed);
prevErr = error;
culErr += error;
if(culErr>40){culErr=40;}
else if(culErr<-40){culErr=-40;}
t2 = millis();
}
