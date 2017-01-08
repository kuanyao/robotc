#pragma config(Motor,  port1,           leftBack,      tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           clawRight,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           clawLeft,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           leftFront,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           rightFront,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           liftMotor2,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           liftMotor3,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           liftMotor4,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           liftMotor1,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightBack,     tmotorVex393_HBridge, openLoop)

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Basic operations
//
// Drive, rotate, left, etc.
//
/////////////////////////////////////////////////////////////////////////////////////////

int clawMotorSpeed = 50;
int rotateMotorSpeed = 50;
int liftMotorSpeed = 50;
int wheelMotorSpeed = 50;

void sendToLiftMotor(int speed) {
		motor[liftMotor1] = speed;
		motor[liftMotor2] = speed;
		motor[liftMotor3] = speed;
		motor[liftMotor4] = speed;
}

void sendToClawMotor(int speed) {
	motor[clawLeft] = speed;
	motor[clawRight] = -speed;
}

void rotate_bot() {
	motor[leftFront]  = speed; 
	motor[rightBack]  = speed; 
	motor[rightFront] = speed;
	motor[leftBack]   = speed;
}

void drive_bot(int speedVeritical, int speedHorizontal) {
	motor[leftFront]  = speedVeritical + speedHorizontal;
	motor[rightBack]  = -1 * (speedVeritical + speedHorizontal);
	motor[rightFront] = speedHorizontal - speedVeritical;
	motor[leftBack]   = speedVeritical - speedHorizontal;
}

void open_bot_claw() {
	sendToClawMotor(clawMotorSpeed);
}

void close_bot_claw() {
	sendToClawMotor(-1 * clawMotorSpeed);
}

void stop_bot_claw() {
	sendToClawMotor(0);
}

void stop_bot_arm() {
	sendToLiftMotor(0);
}

void stop_bot_movement() {
	drive_bot(0, 0);
}

void rotate_bot_counter_clockwise(speed) {
	rotate_bot_clockwise(-1 * speed);
}

void drive_bot_forward(int speed) {
	drive_bot(wheelMotorSpeed, 0);
}

void drive_bot_backward(int speed) {
	drive_bot(-1 * wheelMotorSpeed, 0);
}

void drive_bot_towards_right() {
	drive_bot(0, wheelMotorSpeed);
}

void drive_bot_towards_left() {
	drive_bot(0, -1 * wheelMotorSpeed);
}

void lift_bot_arm() {
	sendToLiftMotor(liftMotorSpeed);
}

void drop_bot_arm() {
	sendToLiftMotor(-1 * liftMotorSpeed);
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                          Pre-Autonomous Functions
//
// You may want to perform some actions before the competition starts. Do them in the
// following function.
//
/////////////////////////////////////////////////////////////////////////////////////////

void pre_auton()
{
  // Set bStopTasksBetweenModes to false if you want to keep user created tasks running between
  // Autonomous and Tele-Op modes. You will need to manage all user created tasks if set to false.
  bStopTasksBetweenModes = true;

	// All activities that occur before the competition starts
	// Example: clearing encoders, setting servo positions, ...
}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 Autonomous Task
//
// This task is used to control your robot during the autonomous phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

int max(int a, int b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

void push_stars_from_high_fence() {
	int timeToMoveForward = 5000;
	int timeToOpenClaw = 2000;
	int timeToLiftArm = 3000;
	int syncCount = 0;

	drive_bot_forward();
	open_bot_claw();
	lift_bot_arm();

	int maxCount = max(max(timeToLiftArm, timeToMoveForward), timeToOpenClaw);

	while (syncCount < maxCount) {
		wait(10);
		syncCount += 10;
		if (syncCount > timeToOpenClaw) {
			stop_bot_claw();
		}
		if (syncCount > timeToLiftArm) {
			stop_bot_arm();
		}
		if (syncCount > timeToMoveForward) {
			stop_bot_movement();
		}
	}

	wait(1000);

	//move bot back
	int timeToMoveBackward = timeToMoveForward / 2;
	int timeToDropArm = timeToLiftArm;
	maxCount = max(timeToMoveBackward, timeToDropArm);
	syncCount = 0;

	drive_bot_backward();
	drop_bot_arm();

	while (syncCount < maxCount) {
		wait(10);
		syncCount += 10;
		if (syncCount > timeToDropArm) {
			stop_bot_arm();
		}
		if (syncCount > timeToMoveBackward) {
			stop_bot_movement();
		}
	}
	
}


task autonomous()
{

}

/////////////////////////////////////////////////////////////////////////////////////////
//
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	while(true){
		int rotateSpeed = 0;
		if (vexRT[Btn5D] == 1) {
			rotateSpeed = -48;
		}
		else if (vexRT[Btn5U] == 1) {
			rotateSpeed = 48;
		}

		motor[leftFront] 	=   vexRT[Ch1] + vexRT[Ch2] + rotateSpeed;
		motor[rightBack] 	= - vexRT[Ch1] - vexRT[Ch2] + rotateSpeed;
		motor[rightFront] = - vexRT[Ch2] + vexRT[Ch1] + rotateSpeed;
		motor[leftBack] 	=   vexRT[Ch2] - vexRT[Ch1] + rotateSpeed;

		sendToLiftMotor(vexRT[Ch2Xmtr2]);

		sendToClawMotor(vexRT[Ch4Xmtr2]);
	}
}


