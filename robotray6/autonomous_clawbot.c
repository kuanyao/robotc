#pragma config(Motor,  port1,           clawLeft,      tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           leftBack,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port3,          rightBack,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port4,           leftFront,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port5,           rightFront,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port6,           liftMotor2,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port7,           liftMotor3,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port8,           liftMotor4,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port9,           liftMotor1,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,           clawRight,     tmotorVex393_HBridge, openLoop)

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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

int clawMotorSpeed = 65;
int rotateMotorSpeed = 50;
int liftMotorSpeed = 80;
int wheelMotorSpeed = 80;

int trigger_autonumous_mode() {
	if (vexRT[Btn6D] == 1 && vexRT[Btn6U] == 1) {
		return 1;
	}
	else {
		return 0;
	}
}

int cancel_autonumous_mode() {
	if (vexRT[Btn6D] == 1 && vexRT[Btn6U] == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

void sendToLiftMotor(int speed) {
	if (speed < 0) {
		speed = speed / 2;
	}
	motor[liftMotor1] = speed;
	motor[liftMotor2] = speed;
	motor[liftMotor3] = speed;
	motor[liftMotor4] = speed;
}

void sendToClawMotor(int speed) {
	motor[clawLeft] = -1 * speed;
	motor[clawRight] = speed;
}

void rotate_bot(int speed) {
	motor[leftFront]  = speed;
	motor[rightBack]  = speed;
	motor[rightFront] = speed;
	motor[leftBack]   = speed;
}

void drive_bot(int speedVeritical, int speedHorizontal ) {
	int adjustSpeed = 10;
	speedHorizontal += adjustSpeed;
	speedVeritical -= adjustSpeed;
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

void rotate_bot_clockwise() {
	rotate_bot(rotateMotorSpeed);
}

void rotate_bot_counter_clockwise() {
	rotate_bot(-1 * rotateMotorSpeed);
}

void drive_bot_forward() {
	drive_bot(wheelMotorSpeed, 0);
}

void drive_bot_backward() {
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

long max(long a, long b) {
	if (a > b) {
		return a;
	} else {
		return b;
	}
}

void push_stars_from_high_fence() {
	int timeToMoveForward = 2500;
	int timeToOpenClaw = 1500;
	int timeToLiftArm = 1500;
	int syncCount = 0;


	drive_bot_forward();
	open_bot_claw();
	lift_bot_arm();

	long maxCount = max(timeToLiftArm, timeToMoveForward);
	maxCount = max(maxCount, timeToOpenClaw);

	while (syncCount <= maxCount) {
		if (cancel_autonumous_mode()) {
			stop_bot_claw();
			stop_bot_movement();
			stop_bot_arm();
			return;
		}
		wait1Msec(10);
		syncCount += 10;
		if (syncCount >= timeToOpenClaw) {
			stop_bot_claw();
		}
		if (syncCount >= timeToLiftArm) {
			stop_bot_arm();
		}
		if (syncCount >= timeToMoveForward) {
			stop_bot_movement();
		}
	}

	wait1Msec(2000);

	//move bot back
	int timeToMoveBackward = timeToMoveForward / 2 - 500;
	int timeToDropArm = timeToLiftArm / 2;
	maxCount = max(timeToMoveBackward, timeToDropArm);
	syncCount = 0;

	drive_bot_backward();
	wait1Msec(500);
	drop_bot_arm();
	//close_bot_claw();

	while (syncCount <= maxCount) {
		if (cancel_autonumous_mode()) {
			stop_bot_arm();
			stop_bot_claw();
			stop_bot_movement();
			return;
		}
		wait1Msec(10);
		syncCount += 10;
		if (syncCount >= timeToDropArm) {
			stop_bot_arm();
		}
		if (syncCount >= timeToMoveBackward) {
			stop_bot_movement();
		}
		if (syncCount >= timeToOpenClaw) {
			stop_bot_claw();
		}
	}

	rotate_bot_clockwise();
	wait1Msec(1000);
	stop_bot_movement();
}


task autonomous()
{
	push_stars_from_high_fence();
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
		if (trigger_autonumous_mode()) {
			push_stars_from_high_fence();
		}

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