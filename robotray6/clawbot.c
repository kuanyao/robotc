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
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

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

task main()
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
