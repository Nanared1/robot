#pragma config(Sensor, in1,    claw_bar_pot,   sensorPotentiometer)
#pragma config(Sensor, in2,    right_mogo_pot, sensorPotentiometer)
#pragma config(Sensor, in3,    gyroscope,      sensorGyro)
#pragma config(Sensor, dgtl7,  dblift_encoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  right_chassis_encoder, sensorQuadEncoder)
#pragma config(Sensor, dgtl11, left_chassis_encoder, sensorQuadEncoder)
#pragma config(Motor,  port1,           right_mobile_goal, tmotorVex393HighSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           claw_bar,      tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           right_lift,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           right_drive_rear, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           right_drive_front, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           left_drive_rear, tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           left_drive_front, tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           left_lift,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port9,           claw_,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          left_mobile_goal, tmotorVex393HighSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define right_enc SensorValue[right_chassis_encoder]
#define left_enc SensorValue[left_chassis_encoder]
#define mogo_rpot SensorValue[right_mogo_pot]
#define mogo_lpot SensorValue[left_mogo_pot]
#define gyro SensorValue[gyroscope]
#define claw_bar_pot SensorValue[claw_bar_sensor]
#define lift_sensor SensorValue[dblift_pot]

void left_drive(int speed){
	motor[left_drive_front] = speed;
	motor[left_drive_rear] = speed;
}
void right_drive(int speed){
	motor[right_drive_front] = speed;
	motor[right_drive_rear] = speed;
}
void drive_chassis(int speed){
	left_drive(speed);
	right_drive(speed);
}
void lift(int speed){
	motor[right_lift] = speed;
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

task main()
{
  while(true){
		if(vexRT[Btn6U]==true){
			motor[left_drive_front]= vexRT[Ch2];
			motor[left_drive_rear]= vexRT[Ch2];
			motor[right_drive_front]= -vexRT[Ch3];
			motor[right_drive_rear]= -vexRT[Ch3];
		}
		else{
			motor[left_drive_front]= -vexRT[Ch3];
			motor[left_drive_rear]= -vexRT[Ch3];
			motor[right_drive_front]= vexRT[Ch2];
			motor[right_drive_rear]= vexRT[Ch2];
		}
		if(vexRT[Btn6UXmtr2] == 1){
			lift(110);
		}else if(vexRT[Btn6DXmtr2] == 1){
			lift(-110);
		}else{
			lift(10);
		}

		if(vexRT[Btn5UXmtr2] == 1){
			claw_lift(120);
		}else if(vexRT[Btn5DXmtr2] == 1){
			claw_lift(-120);
		}else{
			claw_lift(5);
		}

		if(vexRT[Btn8UXmtr2] == 1){
			claw(120);
		}else if(vexRT[Btn8DXmtr2] == 1){
			claw(-120);
		}else{
			claw(0);
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