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

