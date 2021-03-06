#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in8,    ,               sensorGyro)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_2,  leftEncoder,    sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Sensor, I2C_3,  rightEncoder,   sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           h_intake,      tmotorVex393TurboSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           d_left,        tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           y_right,       tmotorVex393TurboSpeed_MC29, openLoop, reversed, encoderPort, I2C_3)
#pragma config(Motor,  port4,           motor1,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           motor2,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           motor3,        tmotorVex393HighSpeed_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port7,           motor4,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           y_left,        tmotorVex393TurboSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port9,           d_right,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          v_intake,      tmotorVex393TurboSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void rightdrive(int speed)
{
	motor[d_right]=speed;
	motor[y_right]=speed;
}
void leftdrive(int speed)
{
	motor[d_left]=speed;
	motor[y_left]=speed;
}

void straight()
{
	if(nMotorEncoder[y_right] == nMotorEncoder[y_left]) // If rightEncoder has counted the same amount as leftEncoder:
	{
		// Move Forward
		rightdrive(110);		    // Right Motor is run at power level 80
		leftdrive(110);		    // Left Motor is run at power level 80
	}
	else if(nMotorEncoder[y_right] > nMotorEncoder[y_left])	// If rightEncoder has counted more encoder counts
	{
		// Turn slightly right
		rightdrive(110);		    // Right Motor is run at power level 60
		leftdrive(115);		    // Left Motor is run at power level 80
	}
	else	// Only runs if leftEncoder has counted more encoder counts
	{
		// Turn slightly left
		rightdrive(115);
		leftdrive(110);
	}
}


void gyro_turn( int degrees10)
{
while(abs(SensorValue[in8]) < degrees10 - 100)
  {
    //...Continue turning
    rightdrive(-100);
    leftdrive(100);
  }
  //Brief brake to eliminate some drift
  rightdrive(5);
  leftdrive(-5);
  wait1Msec(100);

  //Second while loop to move more slowly toward the goal
  while(abs(SensorValue[in8]) != degrees10)
  {
    if(abs(SensorValue[in8]) > degrees10)
    {
      rightdrive(30);
      leftdrive(-30);
    }
    else
    {
      rightdrive(-30);
      leftdrive(30);
    }
  }
  //Stop
  rightdrive(0);
      leftdrive(0);
  wait1Msec(250);
}


void gyro_turnRED( int degrees10)
{
while(abs(SensorValue[in8]) < degrees10 - 100)
  {
    //...Continue turning
    rightdrive(100);
    leftdrive(-100);
  }
  //Brief brake to eliminate some drift
  rightdrive(-5);
  leftdrive(5);
  wait1Msec(100);

  //Second while loop to move more slowly toward the goal
  while(abs(SensorValue[in8]) != degrees10)
  {
    if(abs(SensorValue[in8]) > degrees10)
    {
      rightdrive(-30);
      leftdrive(30);
    }
    else
    {
      rightdrive(30);
      leftdrive(-30);
    }
  }
  //Stop
  rightdrive(0);
      leftdrive(0);
  wait1Msec(250);
}
task main()
{
	//slight turn to the right
	SensorType[in8] = sensorNone;
  wait1Msec(1000);
  //Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
  SensorType[in8] = sensorGyro;
  wait1Msec(2000);
	nMotorEncoder[y_left]=0;
	nMotorEncoder[y_right]=0;

	/*while(abs(nMotorEncoder[y_left]) > -5)
	{
		leftdrive(-75);
		rightdrive(0);
		//wait1Msec(1000);
	}*/
	{
	gyro_turnRED(56);
	leftdrive(0);
	rightdrive(0);
	}
	//nMotorEncoder[y_left]=0;
	//nMotorEncoder[y_right]=0;
	//move up
	while(abs(nMotorEncoder[y_right]) < 470)
	{
		leftdrive(115);
		rightdrive(95);
	}
	{
	leftdrive(0);
	rightdrive(0);
	wait1Msec(1000);
	gyro_turn(700);
	}

	/*nMotorEncoder[y_left]=0;
	nMotorEncoder[y_right]=0;
	while(nMotorEncoder[y_left] < 100)
	{
		leftdrive(127);
		rightdrive(-127);
	}
	leftdrive(0);
	rightdrive(0);*/

	SensorType[in8] = sensorNone;
	SensorType[in8] = sensorGyro;
	wait1Msec(1000);

	gyro_turnRED(505);

	while(abs(nMotorEncoder[y_right]) < 1550)
	{
		leftdrive(100);
		rightdrive(100);
	}
	{
		leftdrive(0);
		rightdrive(0);
	}

	gyro_turnRED(550);

	while(abs(nMotorEncoder[y_right]) < 2500)
	{
		leftdrive(100);
		rightdrive(100);
	}
	{
		leftdrive(0);
		rightdrive(0);
	}

	//leftdrive(127);
	//rightdrive(127);
	//wait1Msec(2000);

}
