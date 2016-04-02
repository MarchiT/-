int line_calibration(int *field, int *line)			//Като аргументи прима адресите на променливите,
{													//в които да се съхраняват стойностите на сензора
	short unsigned int i, refl_val, min, max=0;		//за отражение, когато е върху полето и когато е върху линията.
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
	max=(int)((float)max*0.09);
	min=(int)((float)min*1.01);
	if(!(min < max))
		return -1;
	else
	{
		*field=min;
		*line=max;
		return 0;
	}
}