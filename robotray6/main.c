#pragma config(Motor,  port1,           leftBack,      tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           clawRight,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port3,          rightBack,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port4,           leftFront,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port5,           rightFront,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port6,           liftMotor2,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port7,           liftMotor3,    tmotorVex393_HBridge, openLoop, reversed)
#pragma config(Motor,  port8,           liftMotor4,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port9,           liftMotor1,    tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port10,           clawLeft,     tmotorVex393_HBridge, openLoop)

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX)

//Competition Control and Duration Settings
#pragma competitionControl(Competition)
#pragma autonomousDuration(20)
#pragma userControlDuration(120)

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!

int CLAW_MOTOR_SPEED 	= 65;
int ROTATE_MOTOR_SPEED 	= 50;
int LIFT_MOTOR_SPEED 	= 80;
int WHEEL_MOTOR_SPEED 	= 80;

void sendToLiftMotor(int speed) {
	if (speed < 0) {
		//reduce the drop speed to half
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

void sendToWheelMotor(int speedVeritical, int speedHorizontal, int speedRotation) {
	float wheelAlignmentAdjustmentRatio = 0.1;
	speedHorizontal *= (1 + wheelAlignmentAdjustmentRatio);
	speedVeritical *= (1 - wheelAlignmentAdjustmentRatio);
	motor[leftFront]  = speedVeritical + speedHorizontal + speedRotation;
	motor[rightBack]  = -1 * (speedVeritical + speedHorizontal) + speedRotation;
	motor[rightFront] = speedHorizontal - speedVeritical + speedRotation;
	motor[leftBack]   = speedVeritical - speedHorizontal + speedRotation;
}


void rotate_bot(int speed) {
	sendToWheelMotor(0, 0, speed);
}

void open_bot_claw() {
	sendToClawMotor(CLAW_MOTOR_SPEED);
}

void close_bot_claw() {
	sendToClawMotor(-1 * CLAW_MOTOR_SPEED);
}

void stop_bot_claw() {
	sendToClawMotor(0);
}

void stop_bot_arm() {
	sendToLiftMotor(0);
}

void stop_bot_movement() {
	sendToWheelMotor(0, 0, 0);
}

void rotate_bot_clockwise() {
	rotate_bot(ROTATE_MOTOR_SPEED);
}

void rotate_bot_counter_clockwise() {
	rotate_bot(-1 * ROTATE_MOTOR_SPEED);
}

void drive_bot_forward() {
	sendToWheelMotor(WHEEL_MOTOR_SPEED, 0, 0);
}

void drive_bot_backward() {
	sendToWheelMotor(-1 * WHEEL_MOTOR_SPEED, 0, 0);
}

void drive_bot_towards_right() {
	sendToWheelMotor(0, WHEEL_MOTOR_SPEED, 0);
}

void drive_bot_towards_left() {
	sendToWheelMotor(0, -1 * WHEEL_MOTOR_SPEED, 0);
}

void lift_bot_arm() {
	sendToLiftMotor(LIFT_MOTOR_SPEED);
}

void drop_bot_arm() {
	sendToLiftMotor(-1 * LIFT_MOTOR_SPEED);
}

int trigger_autonomous_mode() {
	if (vexRT[Btn6D] == 1 && vexRT[Btn6U] == 1) {
		return 1;
	}

	return 0;
}

int cancel_autonomous_mode() {
	if (vexRT[Btn6D] == 1 && vexRT[Btn6U] == 0) {
		return 1;
	}

	return 0;
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
	return a > b ? a : b;
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
		if (cancel_autonomous_mode()) {
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
		if (cancel_autonomous_mode()) {
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

void pre_auton()
{
  bStopTasksBetweenModes = true;
}

void doAutonumousMovement() {
	push_stars_from_high_fence();
}

task autonomous()
{
	doAutonumousMovement();
}

task usercontrol()
{
	int liftArmControlThreshold = 10;
	int clawControlThreshold = 10;
	int wheelControlThreshold = 5;

	while(true){
		if (trigger_autonomous_mode()) {
			doAutonumousMovement();
		}

		int rotateDirection = 0;
		if (vexRT[Btn5D] == 1) {
			rotateDirection = -1;
		}
		else if (vexRT[Btn5U] == 1) {
			rotateDirection = 1;
		}

		int wheelStraightMovementInput = vexRT[Ch1];
		int wheelSideMovementInput = vexRT[Ch2];

		if (max(abs(wheelStraightMovementInput), abs(wheelSideMovementInput)) > wheelControlThreshold
			|| rotateDirection != 0) {
			sendToWheelMotor(wheelStraightMovementInput, wheelSideMovementInput, rotateDirection * ROTATE_MOTOR_SPEED);
		}

		int liftControlInput = vexRT[Ch2Xmtr2];
		int clawControlInput = vexRT[Ch4Xmtr2];
		if (abs(liftControlInput) > liftArmControlThreshold) {
			sendToLiftMotor(sgn(liftControlInput) * LIFT_MOTOR_SPEED);
		} else {
			sendToLiftMotor(0);
		}

		if (abs(clawControlInput) > clawControlThreshold) {
			sendToClawMotor(sgn(clawControlInput) * CLAW_MOTOR_SPEED);
		} else {
			sendToClawMotor(0);
		}
	}
}
