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
#define MAX_SPEED 50
#define BUTTON_WAIT 200

int P, I, D, G, errInt, errDeriv, prevErr, errTime, prevErrTime, threshold, i, maxSpeed;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;
  
  maxSpeed = 100;
  D = 0;
  I = 0;
  G = 69;
  P = 69;
  
  runMenu();
}

void loop()
{
  if(startbutton()){
    motor.speed(LEFT_MOTOR, 0);
    motor.speed (RIGHT_MOTOR, 0);
    runMenu();
  }
  
 
  
  int leftSensor = analogRead(LEFT_SENSOR);
  int rightSensor = analogRead(RIGHT_SENSOR);
  int leftErr = (leftSensor < threshold) ? 1 : 0;
  int rightErr = (rightSensor < threshold) ? 1 : 0;

  int totalErr = leftErr - rightErr;
  if (leftErr + rightErr > 0 && totalErr == 0) {
    //off line: set error to be same sign as previous error
    if (prevErr < 0) totalErr = -5;
    if (prevErr >= 0) totalErr = 5;
  }

  if (totalErr != prevErr) {
    errTime = 0;
    prevErrTime = errTime; //take slope from change before previous change
  }

  errDeriv = (totalErr - prevErr) / (errTime + prevErrTime);
  errInt = errInt + totalErr;
  if (errInt > ERR_LIMIT) errInt = ERR_LIMIT;
  if (errInt < -ERR_LIMIT) errInt = -ERR_LIMIT;
  int correct = (G/10.0)*(P * totalErr + I * errInt + D * errDeriv);
  if(correct > 2*maxSpeed){
    correct = 2*maxSpeed;
  }
  else if(correct < -2*maxSpeed){
    correct = -2*maxSpeed;
  }
  int leftMotor, rightMotor;
  leftMotor = (correct > 0) ? maxSpeed : maxSpeed + correct;
  rightMotor = (correct < 0) ? -maxSpeed : -maxSpeed + correct;
  
  motor.speed(LEFT_MOTOR, leftMotor);
  motor.speed (RIGHT_MOTOR, rightMotor);
  
  if(i == 200){
    LCD.clear();
    LCD.home();
    char buffer[1024];
    sprintf(buffer, "%d %d %d", G, P, D);
    LCD.print(buffer);
    LCD.setCursor(0, 1);
    sprintf(buffer, "%d %d %d", correct, leftSensor, rightSensor);
    LCD.print(buffer);
    i = 0;
  }
  
  prevErr = totalErr;
  errTime = errTime + 1;
  i = i + 1;
}

    


void runMenu() {
  
  
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("Speed: ");
    if(stopbutton()){
     maxSpeed = editVal("Speed: ");
    }
    LCD.setCursor(0, 1);
    LCD.print(maxSpeed);
    delay(30);
  }
  
  while(startbutton()){}//wait for button to be unpressed 
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("G: ");
    if(stopbutton()){
     G = editVal("G: ");
    }
    LCD.setCursor(0, 1);
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
    LCD.setCursor(0, 1);
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
    LCD.setCursor(0, 1);
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
    LCD.setCursor(0, 1);v 
    LCD.print(knobVal);
    delay(30);
  }
  while(stopbutton()){}
  return knobVal;
  
}
