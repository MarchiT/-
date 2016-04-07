#include <kipr/botball.h>
#include <time.h>

#define LIGHT_SENSOR_PORT 1
#define START_SENSOR_PORT 0
#define PIPE_PORT 3
#define BOT_PORT 2
#define RIGHT_MOTOR_PORT 1
#define LEFT_MOTOR_PORT 0 
#define SOLAR_ARRAY_PORT 1
#define LIFT_BOT_PORT 0

#define TIME_FOR_FULL_TURN 1150		//P'P8QQP> P=P0P;QQP:P0P=P>, P4P0 QP5 P8P7P<P5QP8 P8 P=P0P3P;P0QP8!
#define PIPE_LOW 1600
#define PIPE_HIGH 370
#define BOT_OPEN 1300
#define BOT_CLOSED 570
#define LIFT_BOT_LOW 1024
#define LIFT_BOT_HIGH 450
#define SOLAR_ARRAY_STARTING 875
#define SOLAR_ARRAY_IN 575
#define SOLAR_ARRAY_OUT 1700

#define SPEED 5.45455 //inch/s

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
	time_t time_start, time_end;
  	//int line_min;
  	//msleep(15000);
	//line_min=line_calibration();
  	msleep(15000);
  	time_start=time(NULL);
  	drive_straight(10);
  	while(analog(LIGHT_SENSOR_PORT)<4000)
      msleep(10);
  	time_end=time(NULL);
  printf("%.5f\n", (double)180.0/(double)(time_end-time_start));
      
	/*int lines_crossed=0, refl_val=0, refl_val_prev=0, line_min;
	set_servo_position(PIPE_PORT, PIPE_LOW);		//PP0QP0P;P=P8 P?P>P7P8QP8P8.
	set_servo_position(BOT_PORT, BOT_OPEN);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	set_servo_position(SOLAR_ARRAY_PORT, SOLAR_ARRAY_STARTING);
	enable_servos();
 	msleep(20000);
	line_min=line_calibration();
	//wait_for_light(0);
  	msleep(15000);
	drive_straight(10);
	while(lines_crossed<3)
	{
		refl_val_prev=refl_val;
		refl_val=analog(LIGHT_SENSOR_PORT);
		if(refl_val>line_min && refl_val_prev<line_min)
			lines_crossed++;
	}
  	drive_straight(850);
  	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
	turn_left(TIME_FOR_FULL_TURN);
	drive_straight(4500);		//P'P8QQP> P=P0P;QQP:P0P=P>, P4P0 QP5 P8P7P<P5QP8 P8 P=P0P3P;P0QP8!
	//drive_left(300);			//
	set_servo_position(BOT_PORT, BOT_CLOSED);	//P!QP8QP:P0 QP>P1P>QP0.
	mav(LEFT_MOTOR_PORT, -500);
	mav(RIGHT_MOTOR_PORT, -1000);
	msleep(300);				//P'P8QQP> P=P0P;QQP:P0P=P>, P4P0 QP5 P8P7P<P5QP8 P8 P=P0P3P;P0QP8!
	drive_backwards(5000);		//
	turn_left(TIME_FOR_FULL_TURN);
	drive_straight(1000);		//P'P8QQP> P=P0P;QQP:P0P=P>, P4P0 QP5 P8P7P<P5QP8 P8 P=P0P3P;P0QP8!
	turn_right(TIME_FOR_FULL_TURN);
	drive_straight(2500);		//P'P8QQP> P=P0P;QQP:P0P=P>, P4P0 QP5 P8P7P<P5QP8 P8 P=P0P3P;P0QP8! PP0P:QP> P8 P2QP8QP:P8 QQP>P9P=P>QQP8 P=P0P4P>P;Q.
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	drive_backwards(1000);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
	turn_right(TIME_FOR_FULL_TURN);
	drive_straight(1000);
	turn_left(TIME_FOR_FULL_TURN);
	drive_straight(5000);
	*/
	disable_servos();
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
	for(i=0; i<250; i++)
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
