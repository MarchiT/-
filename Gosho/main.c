#include <kipr/botball.h>

#define LIGHT_SENSOR_PORT 1
#define BACK_LIGHT_SENSOR_PORT 2
#define START_SENSOR_PORT 0
#define PIPE_PORT 3
#define BOT_PORT 2
#define RIGHT_MOTOR_PORT 1
#define LEFT_MOTOR_PORT 0 
#define SOLAR_ARRAY_PORT 1	//UNUSED
#define LIFT_BOT_PORT 0

#define TIME_FOR_FULL_TURN 1150
#define TIME_FOR_FULL_TURN_LOADED 2100	//UNUSED
#define RAMP_TIME 10000
#TIME_TO_PUSH 5000

#define PIPE_LOW 1712
#define PIPE_HIGH 300
#define BOT_OPEN 1345
#define BOT_CLOSED 450
#define LIFT_BOT_LOW 1024
#define LIFT_BOT_HIGH 480
#define LIFT_BOT_MED 882
#define SOLAR_ARRAY_IN 75	//UNUSED
#define SOLAR_ARRAY_OUT 1400	//UNUSED
#define SOLAR_ARRAY_MOBILE 1990	//UNUSED

#define SPEED 5.45455 //inch/s

#define FRONT_BLACK_MIN 3750
#define BACK_BLACK_MIN 2000

#define CORRECTION_ONE 1800
#define DISTANCE_TO_BOT 15500
#define RETURN_TO_GATE 22000
#define THROUGH_GATE 20000
#define CORRECTION_TWO 12000
#define TO_RAMP 17000
#define START_RAMP 7000
#define CORRECTION_THREE 2500
#define PUSH_PANELS 15000	//UNUSED
#define BACK_OFF 10000	//UNUSED
#define PUSH_DIRT 6000	//UNUSED

void drive_straight(int);
void drive_backwards(int);
void turn_left(int);
void turn_right(int);
void drive_left(int);
void drive_right(int);
void follow_line();
void follow_line_backwards();
int line_calibration();		//UNUSED
void drive_right_backwards(int);
void drive_left_backwards(int);
void initial_positons();
void base_gate();
void gate_bot();
void follow_line_backwards_time( int t);
void deliver_balls();
void go_to_ramp();
void follow_line_time( int t);
void clean_panels();

int main()
{
	//initial_positons();
	//base_gate();
	//gate_bot();
	//deliver_balls();
	//go_to_ramp();
  	enable_servos();	
  	set_servo_position(BOT_PORT, BOT_CLOSED);
  	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
  	drive_straight(2000);
	follow_line_time(RAMP_TIME);
	clean_panels();
	disable_servos();
	ao();
	return 0;
}

void initial_positons()
{
	set_servo_position(PIPE_PORT, PIPE_LOW);		
	set_servo_position(BOT_PORT, BOT_OPEN);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	set_servo_position(SOLAR_ARRAY_PORT, SOLAR_ARRAY_MOBILE);
	enable_servos();
}

void base_gate()
{
	int lines_crossed=0, refl_val=0, refl_val_prev=0;
	printf("Going forward...\n");
	drive_straight(10);
	while(lines_crossed<2)
	{
		refl_val_prev=refl_val;
		refl_val=analog(LIGHT_SENSOR_PORT);
		if(refl_val>FRONT_BLACK_MIN && refl_val_prev<FRONT_BLACK_MIN)
		{
			lines_crossed++;
			printf("%d lines crossed...\n", lines_crossed);
			msleep(250);
		}
		msleep(20);
	}
	drive_straight(CORRECTION_ONE/SPEED); 
}

void gate_bot()
{
	set_servo_position(PIPE_PORT, PIPE_HIGH);
	turn_left(TIME_FOR_FULL_TURN);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
	printf("Going to bot...\n");
	drive_straight(DISTANCE_TO_BOT/SPEED);
	ao();
	printf("Getting bot.\n");
	set_servo_position(BOT_PORT, BOT_CLOSED);
	msleep(1000);
	printf("Lifting bot to medium position...\n");
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_MED);	
	msleep(1000);  
}

void deliver_balls()
{
	drive_backwards(RETURN_TO_GATE/SPEED);			
	turn_left(TIME_FOR_FULL_TURN);
	drive_straight(THROUGH_GATE/SPEED);		
	turn_right(TIME_FOR_FULL_TURN);
	drive_straight(CORRECTION_TWO/SPEED);		
	turn_left(TIME_FOR_FULL_TURN);
	while(analog(LIGHT_SENSOR_PORT) < FRONT_BLACK_MIN)
	{
		printf("Walking to line...\n");
		drive_straight(250);
	}
	//drive_straight(11000/SPEED);
	printf("Leaving balls...\n");
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	ao();
	msleep(1500);  
}

void go_to_ramp()
{
	drive_backwards(TO_RAMP/SPEED);
	turn_right(TIME_FOR_FULL_TURN);
  	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
  	drive_straight(START_RAMP/SPEED);
}

void follow_line_backwards_time( int t)
{
	int elapsed_time=0;
	while(elapsed_time < t)
	{
		follow_line_backwards();
		elapsed_time+=50;
	}
}

void follow_line_time( int t)
{
	int elapsed_time=0;
	while(elapsed_time < t)
	{
		follow_line();
		elapsed_time+=50;
	}
}

void clean_panels()
{
  	ao();
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	msleep(1500);
	drive_straight(CORRECTION_THREE/SPEED);
	turn_left(TIME_FOR_FULL_TURN*0.7);
  	follow_line_time(TIME_TO_PUSH);
  	follow_line_backwards_time(TIME_TO_PUSH);
  	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	/*drive_straight(PUSH_PANELS/SPEED);
	drive_backwards(BACK_OFF);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	turn_left(TIME_FOR_FULL_TURN*2);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
	msleep(1500);
	set_servo_position(BOT_PORT, BOT_OPEN);
	drive_backwards(BACK_OFF/SPEED);
	set_servo_position(BOT_PORT, BOT_CLOSED);
	turn_right(TIME_FOR_FULL_TURN*2);
	drive_straight(PUSH_DIRT/SPEED);*/
}

void drive_straight(int msec){
	mav(LEFT_MOTOR_PORT, 1008);
	mav(RIGHT_MOTOR_PORT, 1000);
	msleep(msec);
}

void drive_backwards(int msec){
	mav(LEFT_MOTOR_PORT, -1008);
	mav(RIGHT_MOTOR_PORT, -1000);
	msleep(msec);
}

void drive_left(int msec){
	mav(LEFT_MOTOR_PORT, 504);
	mav(RIGHT_MOTOR_PORT, 1000);
	msleep(msec);
}

void drive_left_backwards(int msec){
	mav(LEFT_MOTOR_PORT, -1500);
	mav(RIGHT_MOTOR_PORT, -2016);
	msleep(msec);
}

void drive_right(int msec){
	mav(LEFT_MOTOR_PORT, 1008);
	mav(RIGHT_MOTOR_PORT, 500);
	msleep(msec);	
}

void drive_right_backwards(int msec){
	mav(LEFT_MOTOR_PORT, -1016);
	mav(RIGHT_MOTOR_PORT, -1500);
	msleep(msec);	
}

void turn_right(int msec){
	mav(LEFT_MOTOR_PORT, 756);
	mav(RIGHT_MOTOR_PORT, -750);
	msleep(msec);	
}

void turn_left(int msec){
	mav(LEFT_MOTOR_PORT, -756);
	mav(RIGHT_MOTOR_PORT, 750);
	msleep(msec);
}

void follow_line(){
	if(analog(LIGHT_SENSOR_PORT) > FRONT_BLACK_MIN){
		drive_left(50);
	}
	else{
		drive_right(50);
	}
}

void follow_line_backwards(){
	if(analog(BACK_LIGHT_SENSOR_PORT) > BACK_BLACK_MIN){
		drive_left_backwards(50);
	}
	else{
		drive_right_backwards(50);
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
