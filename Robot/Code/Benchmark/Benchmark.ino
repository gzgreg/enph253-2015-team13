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
//  #include <phys253setup.txt>
//  Serial.begin(9600);
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

static int beforeLoc[][2][3] = {
                              {{1, 2, 3}, {2, 3, 4}}
                            };
  static int afterLoc[][2][3] = {
                 {{2, 3, 4}, {3, 4, 5}}
                };
  
  int beforeLocCurr[2][3];
  memcpy(beforeLocCurr, &beforeLoc[0], 2*sizeof(*beforeLoc[0]));
  int afterLocCurr[2][3];
  memcpy(afterLocCurr, &afterLoc[0], 2*sizeof(*afterLoc[0]));
  char buffer[1024];
  sprintf(buffer, "%d %d %d %d", beforeLocCurr[1][2], beforeLocCurr[0][2], beforeLocCurr[1][0]);
  LCD.print(buffer);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  LCD.clear(); LCD.home(); LCD.print(digitalRead(0)); delay(100);
}
