
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
	sendToWheelMotor(WHEEL_MOTOR_SPEED, 0);
}

void drive_bot_backward() {
	sendToWheelMotor(-1 * WHEEL_MOTOR_SPEED, 0);
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