#include <kipr/botball.h>
#include <time.h>

#define LIGHT_SENSOR_PORT 1
#define BACK_LIGHT_SENSOR_PORT 2
#define START_SENSOR_PORT 0
#define PIPE_PORT 3
#define BOT_PORT 2
#define RIGHT_MOTOR_PORT 1
#define LEFT_MOTOR_PORT 0 
#define SOLAR_ARRAY_PORT 1
#define LIFT_BOT_PORT 0

#define TIME_FOR_FULL_TURN 1150
#define TIME_FOR_FULL_TURN_LOADED 2100

#define PIPE_LOW 1712
#define PIPE_HIGH 300
#define BOT_OPEN 1345
#define BOT_CLOSED 550
#define LIFT_BOT_LOW 1024
#define LIFT_BOT_HIGH 480
#define LIFT_BOT_MED 888
#define SOLAR_ARRAY_IN 75
#define SOLAR_ARRAY_OUT 1400
#define SOLAR_ARRAY_MOBILE 1990

#define SPEED 5.45455 //inch/s

#define RAMP_TIME 5000
#define FRONT_BLACK_MIN 3750
#define BACK_BLACK_MIN 2000

#define CORRECTION_ONE 2100
#define DISTANCE_TO_BOT 15000
#define RETURN_TO_GATE 22000
#define THROUGH_GATE 20000
#define CORRECTION_TWO 12000
#define TO_RAMP 15000
#define START_RAMP 2000

void drive_straight(int);
void drive_backwards(int);
void turn_left(int);
void turn_right(int);
void drive_left(int);
void drive_right(int);
void follow_line();
void follow_line_backwards();
int line_calibration();
void drive_right_backwards(int);
void drive_left_backwards(int);
void initial_positons();
void base_gate();
void gate_bot();
void follow_line_backwards_time( int t);
void deliver_balls();
void go_to_ramp();
void follow_line_time( int t);

int main()
{
	initial_positons();
  	
  	base_gate();
  	gate_bot();
	deliver_balls();
	go_to_ramp();
  	//set_servo_position(BOT_PORT, BOT_CLOSED);
  	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
  	msleep(5000);
  	drive_straight(1000);
  	follow_line_time(RAMP_TIME);
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
	while(lines_crossed<3)
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
	//set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
  	ao();
  	msleep(1500);
	turn_right(TIME_FOR_FULL_TURN);
	//drive_backwards(START_RAMP/SPEED);  
}

void follow_line_backwards_time( int t)
{
 	time_t time1, time2;
  	time1=time(NULL);
  	time2=time1;
  	while(time2-time1 < t)
    {
    	follow_line_backwards();
      	time2=time(NULL);
    }
	turn_left(TIME_FOR_FULL_TURN); 
}

void follow_line_time( int t)
{
 	time_t time1, time2;
  	time1=time(NULL);
  	time2=time1;
  	while(time2-time1 < t)
    {
    	follow_line();
      	time2=time(NULL);
    }
	turn_left(TIME_FOR_FULL_TURN); 
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
	if(analog(LIGHT_SENSOR_PORT) > 3900){
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

/*int line_calibration()
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
}*/
