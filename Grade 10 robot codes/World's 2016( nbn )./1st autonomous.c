#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  leftEncoder,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_3,  rightEncoder,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           h_intake,      tmotorVex393TurboSpeed_HBridge, openLoop)
#pragma config(Motor,  port2,           d_left,        tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           y_right,        tmotorVex393TurboSpeed_MC29, openLoop, encoderPort, I2C_2)
#pragma config(Motor,  port4,           motor1,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           motor2,        tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           motor3,        tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port7,           motor4,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           y_left,       tmotorVex393TurboSpeed_MC29, openLoop, encoderPort, I2C_3)
#pragma config(Motor,  port9,           d_right,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          v_intake,      tmotorVex393TurboSpeed_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// Update inteval (in mS) for the flywheel control loop
#define FW_LOOP_SPEED              25

// Maximum power we want to send to the flywheel motors
#define FW_MAX_POWER              127

// encoder counts per revolution depending on motor
#define MOTOR_TPR_269           240.448
#define MOTOR_TPR_393R          261.333
#define MOTOR_TPR_393S          392
#define MOTOR_TPR_393T          627.2
#define MOTOR_TPR_QUAD          360.0

// Structure to gather all the flywheel ralated data
typedef struct _fw_controller {
    long            counter;                ///< loop counter used for debug

    // encoder tick per revolution
    float           ticks_per_rev;          ///< encoder ticks per revolution

    // Encoder
    long            e_current;              ///< current encoder count
    long            e_last;                 ///< current encoder count

    // velocity measurement
    float           v_current;              ///< current velocity in rpm
    long            v_time;                 ///< Time of last velocity calculation

    // TBH control algorithm variables
    long            target;                 ///< target velocity
    long            current;                ///< current velocity
    long            last;                   ///< last velocity
    float           error;                  ///< error between actual and target velocities
    float           last_error;             ///< error last time update called
    float           gain;                   ///< gain
    float           drive;                  ///< final drive out of TBH (0.0 to 1.0)
    float           drive_at_zero;          ///< drive at last zero crossing
    long            first_cross;            ///< flag indicating first zero crossing
    float           drive_approx;           ///< estimated open loop drive

    // final motor drive
    long            motor_drive;            ///< final motor control value
    } fw_controller;

// Make the controller global for easy debugging
static  fw_controller   flywheel;

/*-----------------------------------------------------------------------------*/
/** @brief      Set the flywheen motors                                        */
/** @param[in]  value motor control value                                      */
/*-----------------------------------------------------------------------------*/
void
FwMotorSet( int value )
{
    motor[ motor1 ] = value;
    motor[ motor2 ] = value;
    motor[ motor3 ] = value;
    motor[ motor4 ] = value;
}

/*-----------------------------------------------------------------------------*/
/** @brief      Get the flywheen motor encoder count                           */
/*-----------------------------------------------------------------------------*/
long
FwMotorEncoderGet()
{
    return( -nMotorEncoder[ motor3 ] );
}

/*-----------------------------------------------------------------------------*/
/** @brief      Set the controller position                                    */
/** @param[in]  fw pointer to flywheel controller structure                    */
/** @param[in]  desired velocity                                               */
/** @param[in]  predicted_drive estimated open loop motor drive                */
/*-----------------------------------------------------------------------------*/
void
FwVelocitySet( fw_controller *fw, int velocity, float predicted_drive )
{
    // set target velocity (motor rpm)
    fw->target        = velocity;

    // Set error so zero crossing is correctly detected
    fw->error         = fw->target - fw->current;
    fw->last_error    = fw->error;

    // Set predicted open loop drive value
    fw->drive_approx  = predicted_drive;
    // Set flag to detect first zero crossing
    fw->first_cross   = 1;
    // clear tbh variable
    fw->drive_at_zero = 0;
}

/*-----------------------------------------------------------------------------*/
/** @brief      Calculate the current flywheel motor velocity                  */
/** @param[in]  fw pointer to flywheel controller structure                    */
/*-----------------------------------------------------------------------------*/
void
FwCalculateSpeed( fw_controller *fw )
{
    int     delta_ms;
    int     delta_enc;

    // Get current encoder value
    fw->e_current = FwMotorEncoderGet();

    // This is just used so we don't need to know how often we are called
    // how many mS since we were last here
    delta_ms   = nSysTime - fw->v_time;
    fw->v_time = nSysTime;

    // Change in encoder count
    delta_enc = (fw->e_current - fw->e_last);

    // save last position
    fw->e_last = fw->e_current;

    // Calculate velocity in rpm
    fw->v_current = (1000.0 / delta_ms) * delta_enc * 60.0 / fw->ticks_per_rev;
}

/*-----------------------------------------------------------------------------*/
/** @brief      Update the velocity tbh controller variables                   */
/** @param[in]  fw pointer to flywheel controller structure                    */
/*-----------------------------------------------------------------------------*/
void
FwControlUpdateVelocityTbh( fw_controller *fw )
{
    // calculate error in velocity
    // target is desired velocity
    // current is measured velocity
    fw->error = fw->target - fw->current;

    // Use Kp as gain
    fw->drive =  fw->drive + (fw->error * fw->gain);

    // Clip - we are only going forwards
    if( fw->drive > 1 )
          fw->drive = 1;
    if( fw->drive < 0 )
          fw->drive = 0;

    // Check for zero crossing
    if( sgn(fw->error) != sgn(fw->last_error) ) {
        // First zero crossing after a new set velocity command
        if( fw->first_cross ) {
            // Set drive to the open loop approximation
            fw->drive = fw->drive_approx;
            fw->first_cross = 0;
        }
        else
            fw->drive = 0.5 * ( fw->drive + fw->drive_at_zero );

        // Save this drive value in the "tbh" variable
        fw->drive_at_zero = fw->drive;
    }

    // Save last error
    fw->last_error = fw->error;
}

/*-----------------------------------------------------------------------------*/
/** @brief     Task to control the velocity of the flywheel                    */
/*-----------------------------------------------------------------------------*/
task
FwControlTask()
{
    fw_controller *fw = &flywheel;

    // Set the gain
    fw->gain = 0.0025;

    // We are using Speed geared motors
    // Set the encoder ticks per revolution
    fw->ticks_per_rev = MOTOR_TPR_393S;

    while(1)
        {
        // debug counter
        fw->counter++;

        // Calculate velocity
        FwCalculateSpeed( fw );

        // Set current speed for the tbh calculation code
        fw->current = fw->v_current;

        // Do the velocity TBH calculations
        FwControlUpdateVelocityTbh( fw ) ;

        // Scale drive into the range the motors need
        fw->motor_drive  = (fw->drive * FW_MAX_POWER) + 0.5;

        // Final Limit of motor values - don't really need this
        if( fw->motor_drive >  127 ) fw->motor_drive =  127;
        if( fw->motor_drive < -127 ) fw->motor_drive = -127;

        // and finally set the motor control value
        FwMotorSet( fw->motor_drive );

        // Run at somewhere between 20 and 50mS
        wait1Msec( FW_LOOP_SPEED );
        }
}

// Main user task
#define sr nMotorEncoder
#define mtr motor


//Specify the number of degrees for the robot to turn (1 degree = 10, or 900 = 90 degrees)
int first_gyro = 450;//first gyro turn(shooting position - home base)
int pick1_gyro = 650;//turn to pick up first stack
int face_gyro = 450;//face the net;
int move1 = 800;
int turn1 = 150;
int move2 = 4562;

void leftdrive(int speed)
{
	mtr[d_left]=speed;
	mtr[y_left]=speed;
}

void rightdrive(int speed)
{
	mtr[d_right]=speed;
	mtr[y_right]=speed;
}

void straight()
{
	if(sr[y_right] == sr[y_left]) // If rightEncoder has counted the same amount as leftEncoder:
	{
		// Move Forward
		rightdrive(-110);		    // Right Motor is run at power level 80
		leftdrive(-110);		    // Left Motor is run at power level 80
	}
	else if(sr[y_right] > sr[y_left])	// If rightEncoder has counted more encoder counts
	{
		// Turn slightly right
		rightdrive(-110);		    // Right Motor is run at power level 60
		leftdrive(-115);		    // Left Motor is run at power level 80
	}
	else	// Only runs if leftEncoder has counted more encoder counts
	{
		// Turn slightly left
		rightdrive(-115);
		leftdrive(-110);
	}
}

void shoot(int speed, int pd)
{
	FwVelocitySet( &flywheel, speed, pd );
}

//turn with shooter running
void gyro_turn1(int my_turn)
{
  	//While the absolute value of the gyro is less than the desired rotation - 100...
  	while(abs(SensorValue[in1]) < my_turn - 100)
  	{
    //...Continue turning
    	rightdrive(50);
			leftdrive(-50);
			shoot(160, 1);
  	}
  	//Brief brake to eliminate some drift
  	rightdrive(-5);
		leftdrive(5);
  	wait1Msec(100);

  	//Second while loop to move more slowly toward the goal
  	while(abs(SensorValue[in1]) != my_turn)
  	{
    	if(abs(SensorValue[in1]) > my_turn)
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


//normal turn
void gyro_turn(int my_turn)
{
  	//While the absolute value of the gyro is less than the desired rotation - 100...
  	while(abs(SensorValue[in1]) < my_turn - 100)
  	{
    //...Continue turning
    	rightdrive(50);
			leftdrive(-50);
  	}
  	//Brief brake to eliminate some drift
  	rightdrive(-5);
		leftdrive(5);
  	wait1Msec(100);

  	//Second while loop to move more slowly toward the goal
  	while(abs(SensorValue[in1]) != my_turn)
  	{
    	if(abs(SensorValue[in1]) > my_turn)
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
	//Completely clear out any previous sensor readings by setting the port to "sensorNone"
  SensorType[in1] = sensorNone;
  wait1Msec(1000);
  //Reconfigure Analog Port 8 as a Gyro sensor and allow time for ROBOTC to calibrate it
  SensorType[in1] = sensorGyro;
  wait1Msec(2000);
	sr[y_right]=0;
	sr[y_left]=0;
	wait1Msec(2000);

	while(true){


		FwVelocitySet( &flywheel, 160, 1 );
		wait10Msec(200);
		FwVelocitySet( &flywheel, 160, 1 );
		motor[v_intake]=60;
		motor[h_intake]=60;
		wait10Msec(500);

		//gyro_turn1(first_gyro);
		wait1Msec(1000);
		gyro_turn(pick1_gyro);

		//move before turn
		while(sr[y_right] < move1)
		{
			straight();
		}

		{
			rightdrive(0);
			leftdrive(0);
		}

		gyro_turn(face_gyro);

		/*sr[rightEncoder]=0;
		sr[leftEncoder]=0;
		wait1Msec(2000);
		//turn 1(use gyro)
		while(sr[leftEncoder] < turn1)
		{
			mtr[rightMotor]=-63;
			mtr[leftMotor]=63;
		}

		{
			mtr[rightMotor]=0;
			mtr[leftMotor]=0;
		}*/

		rightdrive(0);
		leftdrive(0);
		wait1Msec(1000);

		//move 2
		while(sr[y_left] < move2)
		{
			straight();
		}

		{
			rightdrive(0);
			leftdrive(0);
		}
	}
}
