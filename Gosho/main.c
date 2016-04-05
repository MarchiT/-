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
void follow_line();
int line_calibration();


int main()
{

	/*drive_backwards(4000);
  	drive_left(3000);*/
  	
  while(1){
  	follow_line();
  }
  
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

 