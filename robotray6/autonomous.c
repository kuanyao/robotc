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

inline int max(int a, int b) {
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

void doAutonumousMovement() {
	push_stars_from_high_fence();
}

task autonomous()
{
	doAutonumousMovement();
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
