#pragma once //only include file once

#include <phys253.h>
#include <LiquidCrystal.h>
#include "menu.h"

//analog inputs
#define LEFT_SENSOR 0
#define RIGHT_SENSOR 1
#define ARM_POT_BASE 2
#define ARM_POT_1 3
#define LEFT_IR 4
#define RIGHT_IR 5
#define L_MARK_SENSOR 6
#define R_MARK_SENSOR 7

//digital inputs
#define LEFT_ENCODER 0
#define RIGHT_ENCODER 1
#define ARM_END 2

//motors
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 0
#define ARM_BASE 2
#define ARM_1 3
#define ARM_2 RCServo0
#define ARM_RELEASE RCServo1

//other constants
#define ARM_ERROR 20 //permissible offset of arm from true value
#define ARM_PID_STOP 10 //value of arm motors that stops the motors
#define ARM_PID_MIN_B 100 //minimum value of arm base motor
#define ARM_PID_MIN_1 100 //minimum value of arm joint 1 motor
#define ARM_TIME_LIMIT 5000 //time for arm to get into position
#define BUTTON_WAIT 200
//states
#define TAPE_FOLLOW 0
#define IR_FOLLOW 1
#define PET_PICKUP 2

unsigned int baseAngle;
unsigned int joint1Angle;
unsigned int joint2Angle;
volatile unsigned int state;
volatile unsigned int petNum;
unsigned int leftRotations;
unsigned int rightRotations;
bool onMarking;
