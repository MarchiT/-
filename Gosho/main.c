#include <kipr/botball.h>

#define LIGHT_SENSOR_PORT 1
#define START_SENSOR_PORT 0
#define PIPE_PORT 3
#define BOT_PORT 2
#define RIGHT_MOTOR_PORT 1
#define LEFT_MOTOR_PORT 0 
#define SOLAR_ARRAY_PORT 1
#define LIFT_BOT_PORT 0

#define TIME_FOR_FULL_TURN 1200		//P'P8QQP> P=P0P;QQP:P0P=P>, P4P0 QP5 P8P7P<P5QP8 P8 P=P0P3P;P0QP8!
#define PIPE_LOW 1040
#define PIPE_HIGH 112
#define BOT_OPEN 1345
#define BOT_CLOSED 550
#define LIFT_BOT_LOW 1024
#define LIFT_BOT_HIGH 480
#define LIFT_BOT_MED 888
#define SOLAR_ARRAY_IN 75
#define SOLAR_ARRAY_OUT 1400
#define SOLAR_ARRAY_MOBILE 1990

#define SPEED 5.45455 //inch/s

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

int main()
{
	int lines_crossed=0, refl_val=0, refl_val_prev=0;//, line_min;
	set_servo_position(PIPE_PORT, PIPE_LOW);		
	set_servo_position(BOT_PORT, BOT_OPEN);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
	set_servo_position(SOLAR_ARRAY_PORT, SOLAR_ARRAY_MOBILE);
	enable_servos();
 	//msleep(20000);
	//line_min=line_calibration();
	//wait_for_light(0);
  	msleep(5000);
	drive_straight(10);
	while(lines_crossed<3)
	{
		refl_val_prev=refl_val;
		refl_val=analog(LIGHT_SENSOR_PORT);
		if(refl_val>3750 && refl_val_prev<3750)
        {
			lines_crossed++;
          	msleep(250);
        }
      	msleep(20);
	}
  	drive_straight(2600/SPEED);
  	//drive_straight(43500/SPEED);
  	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
  	set_servo_position(PIPE_PORT, PIPE_HIGH);
	turn_left(TIME_FOR_FULL_TURN);
	//drive_straight(15500/SPEED);
  	time_t time1, time2;
  	time1=time(NULL);
  	time2=time1;
  	while(time2-time1 < 15.5/SPEED)
    {
    	follow_line();
      	msleep(50);
      	time2=time(NULL);
    }
	ao();
  	set_servo_position(BOT_PORT, BOT_CLOSED);	
  	msleep(1000);
 	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_MED);	
  	msleep(1000);
	drive_backwards(19000/SPEED);			
	turn_left(TIME_FOR_FULL_TURN*2);
	drive_straight(20000/SPEED);		
	turn_right(TIME_FOR_FULL_TURN*2);
	drive_straight(13000/SPEED);		
	turn_left(TIME_FOR_FULL_TURN*2);
	drive_straight(11000/SPEED);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_HIGH);
  	ao();
  	msleep(1500);
	drive_backwards(13100/SPEED);
	set_servo_position(LIFT_BOT_PORT, LIFT_BOT_LOW);
  	ao();
  	msleep(1500);
	turn_left(TIME_FOR_FULL_TURN*2);
	drive_backwards(6000/SPEED);
  	follow_line_backwards();
	turn_left(TIME_FOR_FULL_TURN*2);
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

void drive_left_backwards(int msec){
	mav(LEFT_MOTOR_PORT, -500);
	mav(RIGHT_MOTOR_PORT, -1000);
	msleep(msec);
}

void drive_right(int msec){
	mav(LEFT_MOTOR_PORT, 1000);
	mav(RIGHT_MOTOR_PORT, 500);
	msleep(msec);	
}

void drive_right_backwards(int msec){
	mav(LEFT_MOTOR_PORT, -1000);
	mav(RIGHT_MOTOR_PORT, -500);
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

void follow_line_backwards(){
	if(analog(LIGHT_SENSOR_PORT) > 3750){
		drive_left_backwards(400);
	}
	else{
		drive_right_backwards(400);
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
