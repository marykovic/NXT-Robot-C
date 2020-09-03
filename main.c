#pragma config(Sensor, S1,     sonarRight,      sensorSONAR)
#pragma config(Sensor, S2,     touch,          sensorTouch)
#pragma config(Sensor, S4,     sonarFront,     sensorSONAR)

void turnRight();
void turnLeft();
void goForward();
void goForward(int distance); // distance in cm
void wallTrack();
void rightUturn();
void stopMotors();

int wallInFront();
int wallToRight();

int gameOver();

task main()
{
	while(!gameOver())
	{
		wallTrack();
		if(wallInFront())
			turnLeft();
	}
}

int wallInFront(){
	if (SensorValue(sonarFront) < 20)
		return 1;
	return 0;
}

int wallToRight(){
	if (SensorValue(sonarRight) < 40)
		return 1;
	return 0;
}

void stopMotors(){
	motor[motorA] = 0;
	motor[motorB] = 0;
	wait1Msec(500);
}

void turnRight(){
	nSyncedMotors = synchBA;	//Synch two motors. B is the master motor, and A is the slave motor.
	nSyncedTurnRatio = -100;	//Motor A will recieve the opposite of what motor B recieves
	motor[motorB] = 50;
	wait1Msec(300);
}

void turnLeft(){
	nSyncedMotors = synchAB;	//Synch two motors. A is the master motor, and B is the slave motor.
	nSyncedTurnRatio = -100;	//Motor B will recieve the opposite of what motor A recieves
	motor[motorA] = 50;
	wait1Msec(300);
	stopMotors();
}

void rightUturn(){
	goForward(20);
	turnRight();
	goForward(10);
	turnRight();
	goForward(20);
}

void goForward(){
	nSyncedMotors = synchAB;	//Synch two motors. A is the master motor, and B is the slave motor.
	nSyncedTurnRatio = 100;		//Motor B will recieve 100% of what motor A recieves
	motor[motorA] = 30;
}

void goForward(int distance){
  nSyncedMotors = synchNone;
  nMotorEncoder[motorA] = 0;
  nMotorEncoder[motorB] = 0;	

	nSyncedMotors = synchAB;	//Synch two motors. A is the master motor, and B is the slave motor.
	nSyncedTurnRatio = 100;		//Motor B will recieve 100% of what motor A recieves

	int diameter = 5;					//cm -- Change this value if your wheel size is a different size
	float degrees = distance/(diameter*PI)*360;

	nMotorEncoderTarget[motorA] = (int)degrees;		//Rotates motor A for a specified amount of degrees -- does the same for motorB
	
	motor[motorA] = 50;
	wait1Msec(2000);
}

void wallTrack(){
	
	int power = 70;
	int distFromWall = 24;
	if(SensorValue(sonarRight) < distFromWall)	//while moving towards the wall (sonar placed on the left side)
	{
		//move to the right
		nSyncedMotors = synchAB;	//Synch two motors. A is the master motor, and B is the slave motor.
		nSyncedTurnRatio = 60;		//Motor B will recieve 70% of what motor A recieves
		motor[motorA] = power;
	}
	if(SensorValue(sonarRight) >= distFromWall)	//while moving away from the wall (sonar placed on the right side)
	{
		//move to the left
		nSyncedMotors = synchBA;	//Synch two motors. B is the master motor, and A is the slave motor.
		nSyncedTurnRatio = 60;		//Motor A will recieve 70% of what motor B recieves
		motor[motorB] = power;
	}
}

int gameOver(){
	return SensorValue(touch)
}
