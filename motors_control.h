#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include <kipr/botball.h>

#define LIGHT_SENSOR_PORT 1
#define START_SENSOR_PORT 0
#define PIPE_PORT 3
#define BOT_PORT 2
#define RIGHT_MOTOR_PORT 1
#define LEFT_MOTOR_PORT 0 
#define SOLAR_ARRAY_PORT 1
#define LIFT_BOT_PORT 0

void drive_straight(int);
void drive_backwards(int);
void turn_left(int);
void turn_right(int);
void drive_left(int);
void drive_right(int);

#endif