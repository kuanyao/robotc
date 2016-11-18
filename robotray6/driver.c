#pragma config(Motor,  port2,           leftBack,      tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           leftFront,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port4,           rightBack,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           rightFront,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           liftMotor2,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           liftMotor3,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           liftMotor4,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           liftMotor1,    tmotorVex393_MC29, openLoop)


void setLiftMotor(int speed) {
		motor[liftMotor1] = speed;
		motor[liftMotor2] = speed;
		motor[liftMotor3] = speed;
		motor[liftMotor4] = speed;
}

task main()
{
	while(true){
		int rotateSpeed = 0;
		if (vexRT[Btn5D] == 1) {
			rotateSpeed = -25;
		}
		else if (vexRT[Btn5U] == 1) {
			rotateSpeed = 25;
		}

		motor[leftFront] = -vexRT[Ch1] + vexRT[Ch2] + rotateSpeed;
		motor[rightBack] = - vexRT[Ch1] - vexRT[Ch2] + rotateSpeed;
		motor[rightFront] = - vexRT[Ch2] + vexRT[Ch1] + rotateSpeed;
		motor[leftBack] = vexRT[Ch2] + vexRT[Ch1] + rotateSpeed;

		//lift motors
		if (vexRT[Btn6D] == 1 || vexRT[Btn6DXmtr2] == 1) {
			setLiftMotor(40);
		}
		else if (vexRT[Btn6U] == 1 || vexRT[Btn6UXmtr2] == 1) {
			setLiftMotor(-128);
		}
		else {
			setLiftMotor(0);
		}
	}
}
