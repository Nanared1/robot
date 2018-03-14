#pragma config(Sensor, in1,    claw_bar_pot,   sensorPotentiometer)
#pragma config(Sensor, in2,    right_mogo_pot, sensorPotentiometer)
#pragma config(Sensor, in3,    gyroscope,      sensorGyro)
#pragma config(Sensor, dgtl7,  dblift_encoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  right_chassis_encoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl11, left_chassis_encoder, sensorQuadEncoder)
#pragma config(Motor,  port1,           right_mobile_goal, tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           right_drive_front, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           right_drive_rear, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           claw_,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           left_lift,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           right_lift,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           claw_bar,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           left_drive_front, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           left_drive_rear, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          left_mobile_goal, tmotorVex393HighSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*        Description: Competition template for VEX EDR                      */
/*                                                                           */
/*---------------------------------------------------------------------------*/

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks
  // running between Autonomous and Driver controlled modes. You will need to
  // manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// Set bDisplayCompetitionStatusOnLcd to false if you don't want the LCD
	// used by the competition include file, for example, you might want
	// to display your team name on the LCD in this function.
	// bDisplayCompetitionStatusOnLcd = false;

  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

task autonomous()
{
  // ..........................................................................
  // Insert user code here.
  // ..........................................................................

  // Remove this function call once you have "real" code.
  AutonomousCodePlaceholderForTesting();
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/
void lift(int speed){
	motor[right_lift] = -speed;
	motor[left_lift] = -speed;
}
void claw_lift(int speed){
	motor[claw_bar] = speed;
}
void claw(int speed){
	motor[claw_] = speed;
}
void mogo(int speed){
	motor[right_mobile_goal] = speed;
	motor[left_mobile_goal] = speed;
}
int preset=0;
bool stop_presets(){
	if(vexRT[Btn7U] == 1){
		preset+=1;
	}

	if(preset % 2 == 0)
		return true;
	return false;
}

void claw_preset(){
	while(SensorValue[claw_bar_pot] > 3000){
		claw_lift(80);
	}
	claw_lift(12);
	wait1Msec(50);

	while(SensorValue[claw_bar_pot] < 3000){
		claw_lift(-80);
	}
	claw_lift(12);
	wait1Msec(50);
}
task usercontrol()
{
  while(true){
		if(vexRT[Btn6U]==true){
			motor[left_drive_front]= -vexRT[Ch2];
			motor[left_drive_rear]= -vexRT[Ch2];
			motor[right_drive_front]= -vexRT[Ch3];
			motor[right_drive_rear]= -vexRT[Ch3];
		}
		else{
			motor[left_drive_front]= vexRT[Ch3];
			motor[left_drive_rear]= vexRT[Ch3];
			motor[right_drive_front]= vexRT[Ch2];
			motor[right_drive_rear]= vexRT[Ch2];
		}
		if(vexRT[Btn6UXmtr2] == 1){
			lift(80);
		}else if(vexRT[Btn6DXmtr2] == 1){
			lift(-80);
		}else{
			lift(10);
		}

		if(vexRT[Btn5UXmtr2] == 1){
			claw_lift(120);
		}else if(vexRT[Btn5DXmtr2] == 1){
			claw_lift(-120);
		}else{
			claw_lift(12);
		}

		if(vexRT[Btn8UXmtr2] == 1){
			claw(120);
		}else if(vexRT[Btn8DXmtr2] == 1){
			claw(-120);
		}else if(vexRT[Btn8LXmtr2] == 1  && stop_presets()){
			claw_preset();
		}else{
			claw(-15);
		}

		if(vexRT[Btn7DXmtr2] == 1){
			mogo(120);
		}else if(vexRT[Btn7UXmtr2] == 1){
			mogo(-120);
		}else if(vexRT[Btn7LXmtr2] == 1){
			mogo(85);
		}else if(vexRT[Btn7RXmtr2] == 1){
			mogo(-85);
		}else{
			mogo(0);
		}

		delay(25);
	}
}
