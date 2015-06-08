#include <phys253.h>          
#include <LiquidCrystal.h>    

#define P_PIN 6
#define I_PIN 7
#define LEFT_SENSOR 0
#define RIGHT_SENSOR 1
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1
#define LINE_THRESHOLD 30
#define ERR_LIMIT 10
#define MAX_SPEED 700

int P, I, D, errInt, errDeriv, prevErr;

void setup()
{  
  #include <phys253setup.txt>
  Serial.begin(9600) ;
  
  errInt = 0;
  prevErr = 0;
  D = 0;
}

void loop()
{
  int leftSensor = analogRead(LEFT_SENSOR);
  int rightSensor = analogRead(RIGHT_SENSOR);
  int leftErr = (leftSensor > LINE_THRESHOLD) ? 1 : 0;
  int rightErr = (rightSensor > LINE_THRESHOLD) ? 1 : 0;
  
  int totalErr = leftErr - rightErr;
  if(leftErr + rightErr > 0 && totalErr == 0){
    //off line: set error to be same sign as previous error
    if(prevErr < 0) totalErr = -2;
    if(prevErr > 0) totalErr = 2;
  }
  
  errDeriv = totalErr - prevErr;
  errInt = errInt + totalErr;
  if(errInt > ERR_LIMIT) errInt = ERR_LIMIT;
  if(errInt < -ERR_LIMIT) errInt = -ERR_LIMIT;  
  int correct = P*totalErr + I*errInt + D*errDeriv;
  
  int leftMotor, rightMotor;
  leftMotor = (correct > 0) ? MAX_SPEED : MAX_SPEED - correct;
  rightMotor = (correct < 0) ? MAX_SPEED : MAX_SPEED - correct;
  
  analogWrite(LEFT_MOTOR, leftMotor);
  analogWrite(RIGHT_MOTOR, rightMotor);
  
  prevErr = totalErr;
}
