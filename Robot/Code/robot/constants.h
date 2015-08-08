#pragma once //only include file once

#include <phys253.h>
#include <LiquidCrystal.h>
#include "menu.h"

//analog inputs
#define L_MARK_SENSOR 1
#define LEFT_SENSOR 2
#define RIGHT_SENSOR 3
#define R_MARK_SENSOR 4
#define ARM_POT_BASE 5
#define ARM_POT_1 0
#define LEFT_IR 6
#define RIGHT_IR 7

//digital inputs
#define LEFT_ENCODER 0
#define RIGHT_ENCODER 1
#define ARM_END 2
#define FRONT_BUMPER 3  

//motors
#define LEFT_MOTOR 0
#define RIGHT_MOTOR 1
#define ARM_BASE 2
#define ARM_1 3
#define ARM_2 RCServo0
#define ARM_RELEASE RCServo1

//other constants
#define ARM_ERROR 20 //permissible offset of arm from true value
#define ARM_PID_STOP 10 //value of arm motors that stops the motors
#define ARM_PID_MIN_B 130 //minimum value of arm base motor
#define ARM_PID_MIN_1 125 //minimum value of arm joint 1 motor
#define ARM_TIME_LIMIT 3000 //time for arm to get into position
#define ENCODE_SPD 190 //speed when following encoders
#define SEARCH_SPD 90
#define BUTTON_WAIT 200
//states
#define TAPE_FOLLOW_UP 0
#define TAPE_FOLLOW_DOWN 7
#define IR_FOLLOW_F 1
#define IR_FOLLOW_B 9
#define PET_PICKUP 2
#define PET_DROPOFF 3
#define ZIPLINE 4
#define CATAPULT 5
#define TAPE_SEARCH 6
#define TURN_AROUND 8

unsigned int baseAngle;
unsigned int joint1Angle;
unsigned int joint2Angle;
volatile unsigned int state;
volatile unsigned int petNum;
bool onMarking;
int passedMarkings;
