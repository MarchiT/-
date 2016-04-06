#include <kipr/botball.h>

#define LIGHT_SENSOR_PORT 1
#define START_SENSOR_PORT 0
#define PIPE_PORT 3
#define BOT_PORT 2
#define RIGHT_MOTOR_PORT 1
#define LEFT_MOTOR_PORT 0 
#define SOLAR_ARRAY_PORT 1
#define LIFT_BOT_PORT 0
#define TIME_FOR_FULL_TURN 500		//Чисто налучкано, да се измери и нагласи!
#define PIPE_LOW 1700
#define PIPE_HIGH 370
#define BOT_OPEN 1300
#define BOT_CLOSED 570
#define LIFT_BOT_LOW 1024
#define LIFT_BOT_HIGH 450
#define SOLAR_ARRAY_STARTING 875
#define SOLAR_ARRAY_IN 575
#define SOLAR_ARRAY_OUT 1700

void drive_straight(int);
void drive_backwards(int);
void turn_left(int);
void turn_right(int);
void drive_left(int);
void drive_right(int);
void follow_line();
int line_calibration();

int main()
{
	/*
	drive_backwards(4000);
	drive_left(3000);
	while(1){
	follow_line();
	*/
	int lines_crossed=0, refl_val=0, refl_val_prev=0, line_min;
	set_servo_position(PIPE_PORT, PIPE_LOW);		//Начални позиции.
	set_servo_position(BOT_PORT, BOT_OPEN);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	set_servo_position(SOLAR_ARRAY_PORT, SOLAR_ARRAY_STARTING);
	enable_seros();
	line_min=line_calibration();
	wait_for_light(0);
	drive_straight(10);
	while(lines_crossed<3)
	{
		refl_val_prev=refl_val;
		refl_val=analog(LIGHT_SENSOR_PORT);
		if(refl_val>line_min && refl_val_prev<line_min)
			lines_crossed++;
	}
	turn_left(TIME_FOR_FULL_TURN);
	drive_straight(1000);		//Чисто налучкано, да се измери и нагласи!
	drive_left(300);			//
	set_servo_position(BOT_PORT, BOT_CLOSED);	//Стиска робота.
	mav(LEFT_MOTOR_PORT, -500);
	mav(RIGHT_MOTOR_PORT, -1000);
	msleep(300);				//Чисто налучкано, да се измери и нагласи!
	drive_backwards(1000);		//
	turn_left(TIME_FOR_FULL_TURN);
	drive_straight(500);		//Чисто налучкано, да се измери и нагласи!
	turn_right(TIME_FOR_FULL_TURN);
	drive_straight(1000);		//Чисто налучкано, да се измери и нагласи! Както и всички стойности надолу.
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	drive_backwards(500);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
	turn_right(TIME_FOR_FULL_TURN);
	drive_straight(500);
	turn_left(TIME_FOR_FULL_TURN);
	drive_straight(5000);


	ao();
	return 0;
}



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
	mav(LEFT_MOTOR_PORT, 500);
	mav(RIGHT_MOTOR_PORT, 1000);
	msleep(msec);
}

void drive_right(int msec){
	mav(LEFT_MOTOR_PORT, 1000);
	mav(RIGHT_MOTOR_PORT, 500);
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

void follow_line(){
	if(analog(LIGHT_SENSOR_PORT) > 3900){
		drive_left(400);
	}
	else{
		drive_right(400);
	}
}

int line_calibration()
{
	short unsigned int i, refl_val, min, max=0, on_line;
	mav(LEFT_MOTOR_PORT, 1000);
	mav(RIGHT_MOTOR_PORT, 1000);
	min=analog(LIGHT_SENSOR_PORT);
	for(i=0; i<500; i++)
	{
		refl_val=analog(LIGHT_SENSOR_PORT);
		if(refl_val < min)
			min=refl_val;
		else if(refl_val > max)
		{
			max=refl_val;
		}
		msleep(10);
	}
	ao();
	if(!(min < max))
		return -1;
	else
	{
		on_line=(min+max)/2;
		printf("\nAko analog(LIGHT_SENSOR_PORT) > %d, znachi Gosho e na liniata.", on_line);
		return on_line;
	}
}
