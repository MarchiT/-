int line_calibration()
{
	short unsigned int i, refl_val, min, max=0, on_line;
	mav(LEFT_MOTOR_PORT, 1000);
	mav(RIGHT_MOTOR_PORT, 1000);
	min=analog(LIGHT_SENSOR_PORT);
	for(i=0; i<500; i++)
	{
		refl_val=analog(LIGHT_SENSOR_PORT);
		if(refl_val < min);
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
		printf("\nAko analog(LIGHT_SENSOR_PORT) < %d, znachi Gosho e na liniata.", on_line);
		return on_line;
	}
}
