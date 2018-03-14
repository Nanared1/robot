#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl9,  sRight,         sensorQuadEncoder)
#pragma config(Sensor, dgtl11, sLeft,          sensorQuadEncoder)
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

task main()
{
  //Completely clear out any previous sensor readings by setting the port to "sensorNone"
  SensorType[in1] = sensorNone;
  wait1Msec(1000);
  //Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
  SensorType[in1] = sensorGyro;
  wait1Msec(2000);


  //Specify the number of degrees for the robot to turn (1 degree = 10, or 900 = 90 degrees)
  int degrees10 = 1030;

  //While the absolute value of the gyro is less than the desired rotation - 100...
  while(abs(SensorValue[in1]) < degrees10 - 100)
  {
    //...Continue turning
    motor[rightFront] = 50;
    motor[rightBack] = 50;
    motor[leftFront] = -50;
    motor[leftBack] = -50;
  }
  //Brief brake to eliminate some drift
  motor[rightFront] = -5;
  motor[rightBack] = -5;
  motor[leftFront] = 5;
  motor[leftBack] = 5;
  wait1Msec(100);

  //Second while loop to move more slowly toward the goal
  while(abs(SensorValue[in1]) != degrees10)
  {
    if(abs(SensorValue[in1]) > degrees10)
    {
      motor[rightFront] = -30;
      motor[rightBack] = -30;
      motor[leftFront] = 30;
      motor[leftBack] = 30;
    }
    else
    {
      motor[rightFront] = 30;
      motor[rightBack] = 30;
      motor[leftFront] = -30;
      motor[leftBack] = -30;
    }
  }
  //Stop
  motor[rightFront] = 0;
  motor[rightBack] = 0;
  motor[leftFront] = 0;
  motor[leftBack] = 0;
  wait1Msec(250);
}