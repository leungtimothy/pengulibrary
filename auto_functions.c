

/**
 * Use this function command the robot to rotate about its Z axis using the data obtained via the gyroscope.
 *
 * @param turnDirection Choose LEFT or RIGHT for direction of turn.
 * @param targetDegrees the amount of degrees the robot will rotate relative to its original bearing.
 *
 */
void driveGyroTurn (int turnDirection, int targetDegrees) {
	// initialize PD loop variables
	float kp = 2;
	float kd = 0.15;
	int error = targetDegrees;
	int previousError = targetDegrees;
	int deltaError = 0;
	int drivePower = 0;

	// min/max power
	int maxPower = 90;
	int minPower = 25;

	// finish check variables
	int timeDelta = 0;
	int timePrevious = 0;
	bool isAtTarget = false;
	bool isTimerOn = false;

	// initialize gyro data variables
	int targetReading = gyroGet(gyro);

	// get gyroscope target reading
	if (turnDirection == 7)
		targetReading += targetDegrees;
	else if (turnDirection == 8)
	 	targetReading -= targetDegrees;



	// run motors until target is within 1/2 degree certainty
	while (!isAtTarget) {
		error = targetReading - gyroGet(gyro);	// calculate error
		deltaError = error - previousError;						// calculate deltaError
		drivePower = error * kp + deltaError * kd;				// calculate PD loop output
		previousError = error;									// set new previous error

		// cap between min and max power
		drivePower = drivePower < minPower && drivePower > 0 ? minPower : drivePower > -minPower && drivePower < 0 ? -minPower : drivePower;
		drivePower = drivePower > maxPower ? maxPower : drivePower < -maxPower ? -maxPower : drivePower;

		driveSetChannel(YAW, -drivePower);

		printf("Gyro Value: %d\tError: %d\r\n", gyroGet(gyro), error);

		// check for finish
		if (abs(error) < 1 && !isTimerOn) {			// if robot is within 1 degree of target and timer flag is off
			timeDelta = millis() - timePrevious;
			isTimerOn = true;						// set timer flag to indicate timer is running
			timePrevious = millis();
		}
		else if (abs(error) > 1)					// if robot is not within 1 degree of target
			isTimerOn = false;						// timer flag is reset

		if (timeDelta > 1000 && isTimerOn)			// if the timer is over 250ms and timer flag is true
			isAtTarget = true;						// set boolean to complete while loop

		delay(20);
	}

	driveStop();
}
