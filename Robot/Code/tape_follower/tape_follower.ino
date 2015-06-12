//Note that the 

#include <phys253.h>
#include <LiquidCrystal.h>

#define CONFIG_PIN 6
#define CORRECT_PIN 7
#define LEFT_SENSOR 0
#define RIGHT_SENSOR 1
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 0
#define ERR_LIMIT 10
#define MAX_SPEED 160
#define BUTTON_WAIT 200

int P, I, D, G, errInt, errDeriv, prevErr, errTime, prevErrTime, threshold, i, flash;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;

  D = 69;
  I = 69;
  G = 69;
  P = 69;
  runMenu();
}

void loop()
{  
  int leftErr = (analogRead(LEFT_SENSOR) < threshold) ? 1 : 0;
  int rightErr = (analogRead(RIGHT_SENSOR) < threshold) ? 1 : 0;

  int totalErr = leftErr - rightErr;
  if (leftErr == 1 && rightErr == 1) {
    //off line: set error to be same sign as previous error
    if (prevErr < 0) totalErr = -3;
    if (prevErr >= 0) totalErr = 3;
  }

  if (totalErr != prevErr) {
    errTime = 0;
    prevErrTime = errTime; //take slope from change before previous change
  }

  errDeriv = (totalErr - prevErr) / (errTime + prevErrTime);
  errInt = errInt + totalErr;
  if (errInt > ERR_LIMIT) errInt = ERR_LIMIT;
  if (errInt < -ERR_LIMIT) errInt = -ERR_LIMIT;
  int correct = G*(P * totalErr + I * errInt + D * errDeriv);
  if(correct > MAX_SPEED * 2) correct = 511;
  if(correct < -MAX_SPEED * 2) correct = -511;
  
  int leftMotor, rightMotor;
  leftMotor = (correct > 0) ? MAX_SPEED : MAX_SPEED + correct;
  rightMotor = (correct < 0) ? -MAX_SPEED : -MAX_SPEED + correct;
  
  motor.speed(LEFT_MOTOR, leftMotor);
  motor.speed (RIGHT_MOTOR, rightMotor);
  
  if(i == 100){
    if(startbutton()){
      if(startbutton()){
        if(startbutton()){
          motor.speed(LEFT_MOTOR, 0);
          motor.speed (RIGHT_MOTOR, 0);
          runMenu();
        }
      }
    }
    
    if(stopbutton()){
      if(stopbutton()){
        if(stopbutton()){
          motor.speed(LEFT_MOTOR, 0);
          motor.speed (RIGHT_MOTOR, 0);
          delay(BUTTON_WAIT);
          while(!stopbutton()){
            int leftSensor = analogRead(LEFT_SENSOR);
            int rightSensor = analogRead(RIGHT_SENSOR);
            LCD.clear();
            LCD.home();
            LCD.print(leftSensor);
            LCD.setCursor(0, 1);
            LCD.print(rightSensor);
            delay(50);
          }  
        }
      }
      delay(BUTTON_WAIT);
    }
    
    LCD.clear();
    LCD.home();
    char buffer[1024];
    sprintf(buffer, "%d %d %d", G, P, D);
    LCD.print(buffer);
    LCD.setCursor(0, 1);
    sprintf(buffer, "%d %d %d", correct, leftMotor, rightMotor);
    LCD.print(buffer);
    i = 0;
  }
  
  prevErr = totalErr;
  errTime = errTime + 1;
  i = i + 1;
}

    


void runMenu() {
  while(startbutton()){}//wait for button to be unpressed 
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("G: ");
    if(stopbutton()){
     G = editVal("G: ");
    }
    LCD.setCursor(3, 0);
    LCD.print(G);
    delay(30);
  }
  
  while(startbutton()){}//wait for button to be unpressed 
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("P: ");
    if(stopbutton()){
      P = editVal("P: ");
    }
    LCD.setCursor(3, 0);
    LCD.print(P);
    delay(30);
  }
  
  while(startbutton()){}//wait for button to be unpressed 
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("D: ");
    if(stopbutton()){
      D = editVal("D: ");
    }
    LCD.setCursor(3, 0);
    LCD.print(D);
    delay(30);
  }
  
  while(startbutton()){}//wait for button to be unpressed 
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("Thresh: ");
    if(stopbutton()){
      threshold = editVal("Thresh: ");
    }
    LCD.setCursor(0, 1);
    LCD.print(threshold);
    delay(30);
  }
  
  delay(BUTTON_WAIT);
  errInt = 0;
  prevErr = 0;
  errTime = 0;
  prevErrTime = 0;
  i = 100;
}

int editVal(String val) {
  while(stopbutton()){}
  int flash = 0;
  int knobVal;
  while(!stopbutton()){
    flash++;
    knobVal = knob(CONFIG_PIN);
    if(flash%10>5){
      LCD.clear();
      }
    else{
      LCD.home();
      LCD.print(val);
      }          
    LCD.setCursor(0, 1);
    LCD.print(knobVal);
    delay(30);
  }
  while(stopbutton()){}
  return knobVal;
  
}
