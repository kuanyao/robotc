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
#include "motor_basic.c"
#include "bot_operation.c"
#include "autonomous.c"

int CLAW_MOTOR_SPEED 	= 65;
int ROTATE_MOTOR_SPEED 	= 50;
int LIFT_MOTOR_SPEED 	= 80;
int WHEEL_MOTOR_SPEED 	= 80;

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
//                                 User Control Task
//
// This task is used to control your robot during the user control phase of a VEX Competition.
// You must modify the code to add your own robot specific commands here.
//
/////////////////////////////////////////////////////////////////////////////////////////

task usercontrol()
{
	int liftArmControlThreshold = 10;
	int clawControlThreshold = 10;
	int wheelControlThreshold = 5;

	while(true){
		if (trigger_autonumous_mode()) {
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
			drive_bot(wheelStraightMovementInput, wheelSideMovementInput, rotateDirection * rotateMotorSpeed);
		}

		int liftControlInput = vexRT[Ch2Xmtr2];
		int clawControlInput = vexRT[Ch4Xmtr2];
		if (abs(liftControlInput) > liftArmControlThreshold) {
			sendToLiftMotor(sgn(liftControlInput) * liftMotorSpeed);
		} else {
			sendToLiftMotor(0);
		}

		if (abs(clawControlInput) > clawControlThreshold) {
			sendToClawMotor(sgn(clawControlInput) * clawMotorSpeed);
		} else {
			sendToClawMotor(0);
		}
	}
}
