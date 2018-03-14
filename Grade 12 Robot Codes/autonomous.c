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

#include "library/pid_h.h"
#define right_enc SensorValue[right_chassis_encoder]
#define left_enc SensorValue[left_chassis_encoder]

void left_drive(int speed){
	motor[left_drive_front] = speed;
	motor[left_drive_rear] = speed;
}

void right_drive(int speed){
	motor[right_drive_front] = speed;
	motor[right_drive_rear] = speed;
}
void mobile_goal_intake(int speed){
	motor[right_mobile_goal] = speed;
	motor[left_mobile_goal] = speed;
}

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

void drive_chassis_init(){
	right_enc = 0;
	left_enc = 0;
}

pid drivebase;
void drivebase_PID_init(){
	drivebase.kP = 0.5;
	drivebase.kI = 0.0009;
	drivebase.kD = 2.5;
}
void drive_chassis(int speed){
	left_drive(speed);
	right_drive(speed);
}
void setDriveTarget(int distance){
	drivebase.target = distance;
}

task drivebase_PID() {
	drivebase_PID_init();
	while(true){

		drivebase.error = drivebase.target - abs(left_enc);

		/*if(abs(drivebase.error)<250)
			drivebase.integral += drivebase.error;
		else
			drivebase.integral = 0;*/

		if(abs(drivebase.error)<200)
			drivebase.integral += 1;
		else
			drivebase.integral = 0;


		if(drivebase.error == 0) { drivebase.integral = 0; }

		drivebase.derivative = drivebase.error - drivebase.last_error;
		drivebase.last_error = drivebase.error;

		drive_chassis((drivebase.kP*drivebase.error)+(drivebase.integral*drivebase.kI)+(drivebase.derivative*drivebase.kD));
		delay(25);
	}
}
void open(){

}
task main()
{
	drive_chassis_init();
	startTask(drivebase_PID);
	while(1){
		mobile_goal_intake(-100);
		wait1Msec(1000);
		setDriveTarget(1000);
	}
}