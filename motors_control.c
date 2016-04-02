#include "motors_control.h"

void drive_straight(int msec){
	mav(LEFT_MOTOR_PORT, 1000);
	mav(RIGHT_MOTOR_PORT, 1000);
	msleep(msec);
}

void drive_backwards(int msec){
	mav(LEFT_MOTOR_PORT, -1000);
	mav(RIGHT_MOTOR_PORT, -1000);
	msleep(msec);
}

void drive_left(int msec){
	mav(LEFT_MOTOR_PORT, 800);
	mav(RIGHT_MOTOR_PORT, 1000);
	msleep(msec);	
}

void drive_right(int msec){
	mav(LEFT_MOTOR_PORT, 1000);
	mav(RIGHT_MOTOR_PORT, 800);
	msleep(msec);	
}

void turn_right(int msec){
	mav(LEFT_MOTOR_PORT, 750);
	mav(RIGHT_MOTOR_PORT, -750);
	msleep(msec);	
}

void turn_left(int msec){
	mav(LEFT_MOTOR_PORT, -750);
	mav(RIGHT_MOTOR_PORT, 750);
	msleep(msec);	
}