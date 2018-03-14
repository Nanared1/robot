#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    ,               sensorGyro)
#pragma config(Sensor, dgtl3, leftEncoder,          sensorQuadEncoder).
#pragma config(Sensor, dgtl11,  rightEncoder,         sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           leftFront,     tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           Motor1,        tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port3,           Motor2,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           Motor3,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           Motor4,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           Motor5,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           rightBack,     tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           leftBack,      tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           Intake,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          rightFront,    tmotorVex393HighSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//



void straight()
{
	if(SensorValue[rightEncoder] == SensorValue[leftEncoder]) // If rightEncoder has counted the same amount as leftEncoder:
	{
		// Move Forward
		motor[rightBack] = -110;		    // Right Motor is run at power level 80
		motor[leftBack]  = -110;		    // Left Motor is run at power level 80
	}
	else if(SensorValue[rightEncoder] > SensorValue[leftEncoder])	// If rightEncoder has counted more encoder counts
	{
		// Turn slightly right
		motor[rightBack] = -108;		    // Right Motor is run at power level 60
		motor[leftBack]  = -110;		    // Left Motor is run at power level 80
	}
	else	// Only runs if leftEncoder has counted more encoder counts
	{
		// Turn slightly left
		motor[rightBack] = -110;		    // Right Motor is run at power level 80
		motor[leftBack]  = -108;		    // Left Motor is run at power level 60
	}
}
task main()
{
	{
  SensorValue[rightEncoder]=0;
  while(SensorValue[rightEncoder] < 170)
  {
    motor[rightBack] = -100;
    motor[leftBack] = -100;
  }
  	motor[rightBack] = 0;
    motor[leftBack] = 0;
  }
  //Completely clear out any previous sensor readings by setting the port to "sensorNone"
  SensorType[in1] = sensorNone;
  wait1Msec(500);
  //Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
  SensorType[in1] = sensorGyro;
  wait1Msec(1000);


  //Specify the number of degrees for the robot to turn (1 degree = 10, or 900 = 90 degrees)
  int degrees10 = 870;

  //While the absolute value of the gyro is less than the desired rotation - 100...
  while(abs(SensorValue[in1]) < degrees10 - 100)
  {
    //...Continue turning
    motor[rightBack] = 70;
    motor[leftBack] = -70;
  }
  //Brief brake to eliminate some drift
  motor[rightBack] = -5;
  motor[leftBack] = 5;
  wait1Msec(100);

  //Second while loop to move more slowly toward the goal
  while(abs(SensorValue[in1]) != degrees10)
  {
    if(abs(SensorValue[in1]) > degrees10)
    {
      motor[rightBack] = -70;
      motor[leftBack] = 70;
    }
    else
    {
      motor[rightBack] = 70;
      motor[leftBack] = -70;
    }
  }
  {
  //Stop
  motor[rightBack] = 0;
  motor[leftBack] = 0;
  wait1Msec(250);
  }
  SensorType[in1] = sensorNone;
  wait1Msec(500);
  //Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
  SensorType[in1] = sensorGyro;
  wait1Msec(1000);

  SensorValue[rightEncoder] = 0;	  // Set the encoder so that it starts counting at 0
	SensorValue[leftEncoder]  = 0;	  // Set the encoder so that it starts counting at 0

	while(SensorValue[rightEncoder] < 2920)		// Creates an infinite loop, since "true" always evaluates to true
	{
		{
			straight();
		}
	}
	while(abs(SensorValue[in1]) < 1025 - 100)
  {
    //...Continue turning
    motor[rightBack] = -60;
    motor[leftBack] = 60;
  }
  //Brief brake to eliminate some drift
  motor[rightBack] = 5;
  motor[leftBack] = -5;
  wait1Msec(100);

  //Second while loop to move more slowly toward the goal
  while(abs(SensorValue[in1]) != 1025)
  {
    if(abs(SensorValue[in1]) > 1025)
    {
      motor[rightBack] = 60;
      motor[leftBack] = -60;
    }
    else
    {
      motor[rightBack] = -60;
      motor[leftBack] = 60;
    }
  }
  {
  //Stop
  motor[rightBack] = 0;
  motor[leftBack] = 0;
  wait1Msec(250);
  }
  /*{
  SensorValue[rightEncoder]=0;
  while(SensorValue[rightEncoder] < 60)
  {
    motor[rightBack] = -100;
    motor[leftBack] = -100;
  }
  	motor[rightBack] = 0;
    motor[leftBack] = 0;
  }*/
}