//Note that the 

#include <phys253.h>
#include <LiquidCrystal.h>

#define CONFIG_PIN 6
#define CORRECT_PIN 7
#define LEFT_SENSOR 2
#define RIGHT_SENSOR 3
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1
#define ERR_LIMIT 10
#define MAX_SPEED 255
#define BUTTON_WAIT 200

int P, I, D, G, errInt, errDeriv, prevErr, errTime, prevErrTime, threshold, i;

void setup()
{
#include <phys253setup.txt>
  Serial.begin(9600) ;

  D = 0;
  I = 0;
  G = 0;
  P = 0;
  runMenu();
}

void loop()
{
  if(startbutton()){
    motor.speed(LEFT_MOTOR, 0);
    motor.speed (RIGHT_MOTOR, 0);
    runMenu();
  }
  
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
    }
  }
  
  int leftSensor = analogRead(LEFT_SENSOR);
  int rightSensor = analogRead(RIGHT_SENSOR);
  int leftErr = (leftSensor > threshold) ? 1 : 0;
  int rightErr = (rightSensor > threshold) ? 1 : 0;

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
  int correct = G*(P * totalErr + I * errInt + D * errDeriv);

  int leftMotor, rightMotor;
  leftMotor = (correct > 0) ? MAX_SPEED : MAX_SPEED - correct;
  rightMotor = (correct < 0) ? -MAX_SPEED : -MAX_SPEED + correct;
  
  motor.speed(LEFT_MOTOR, leftMotor);
  motor.speed (RIGHT_MOTOR, rightMotor);
  
  if(i == 100){
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
  delay(BUTTON_WAIT);
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("G: ");
    G = knob(CONFIG_PIN);
    LCD.setCursor(3, 0);
    LCD.print(G);
    delay(50);
  }
  
  delay(BUTTON_WAIT);
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("P: ");
    P = knob(CONFIG_PIN);
    LCD.setCursor(3, 0);
    LCD.print(P);
    delay(50);
  }
  
  delay(BUTTON_WAIT);
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("D: ");
    D = knob(CONFIG_PIN);
    LCD.setCursor(3, 0);
    LCD.print(D);
    delay(50);
  }
  
  delay(BUTTON_WAIT);
  while(!startbutton()){
    LCD.clear();
    LCD.home();
    LCD.print("Thresh: ");
    threshold = knob(CONFIG_PIN);
    LCD.setCursor(0, 1);
    LCD.print(threshold);
    delay(50);
  }
  
  delay(BUTTON_WAIT);
  errInt = 0;
  prevErr = 0;
  errTime = 0;
  prevErrTime = 0;
  i = 100;
}
