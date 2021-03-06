#pragma config(Motor,  motorA,          xGan,          tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorB,          extrude,       tmotorNXT, PIDControl, reversed, encoder)
#pragma config(Motor,  motorC,          yGan,          tmotorNXT, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define enc nMotorEncoder
#define wait wait1Msec
#define buttons nNxtButtonPressed

/*    MODES      */
#define DORMANT 0
#define MANUAL 1
#define PRINTING 2
#define DEBUG 3

#include "JoystickDriver.c"

void setXPID(float p, float i, float d);
void setYPID(float p, float i, float d);
void reset();


float px = 0.5, ix = 0.0, dx = 0.0, py = 0.5, iy = 0.0, dy = 0.0;

task main() {
	int mode = DORMANT;
	const int numModes = 4;
	int lastButton = 0;
	int modeView = 0;
	string modeNames[numModes] = {"DORMANT", "MANUAL", "PRINTING", "DEBUG"};

	nPowerDownDelayMinutes = 500000;
	reset();

	setXPID(0.53, 0.0, 0.0); // TODO: Move these to MoveGantry and destroy global variables .530 bottom is .5
	setYPID(0.5, 0.0, 0.0);


	/*Manual Control Variables*/

	int extruderSpeed = 80;
	bool prevStateRB = false;
	bool prevStateRT = false;

	/*Command Variables*/
	typedef struct {
		float x; // yPosition
		float y; // x position
		float z; // z position
		float e; // extrusion speed
	} Command;

	Command active; // current command being executed

	bool activeFull = false; //whether commands have been red

	/*PID VARIABLES */
	int outMax = 80, outMin = -80;
	int lastX = 0, lastY = 0;
	float xIVal = 0, yIVal = 0;
	int counter = 0, distance = 1000;
	float px = 0.5, ix = 0.0, dx = 0.0, py = 0.5, iy = 0.0, dy = 0.0;


	/* PARSING VARIABLES */
	TFileHandle parsingHandle;
	TFileIOResult parsingIOResult;
	const string parsingName = "part.txt";
	const float SCALE_X = 0.015119055;
	const float SCALE_Y = 0.016798944;
	short parsingSize = 100;

	const unsigned byte SP = 0x20;
	const unsigned byte LF = 0x0a;
	const unsigned byte CR =	0x0d;

	OpenRead(parsingHandle, parsingIOResult, parsingName, parsingSize);

	while(true) {

		/********************************************************************************
		*                                MODE SELECTION
		********************************************************************************/
		if(lastButton != -1 && buttons == -1) { //if you
			if(lastButton == 3) {
				mode = modeView;
			}
			else if(lastButton == 2) {
				if(modeView == 0) modeView = numModes - 1;
				else modeView--;
			}
			else if(lastButton == 1) {
				if(modeView == numModes - 1) modeView = 0;
				else modeView++;
			}
		}


		if(mode == DORMANT) {
			motor(xGan) = 0;
			motor(yGan) = 0;
		}

		/********************************************************************************
		*                                AUTO PRINTING
		********************************************************************************/

		if(mode == PRINTING) {

			if(!activeFull) { //Read Single Line store it into active command
				if(parsingIOResult != ioRsltEndOfFile){
					char type, skipG, checker;
					string numStr = "";
					float typeVal;
					motor[extrude] = 100;

					for(int i = 0; i < 3; i++){
						ReadByte(parsingHandle, parsingIOResult, skipG);
					}

					do{
						ReadByte(parsingHandle, parsingIOResult, type);


						int float_index = 0;
						char temp[12];
						do{
							ReadByte(parsingHandle, parsingIOResult, temp[float_index]);
							checker = temp[float_index];
							float_index++;
						}while(checker != SP && checker != CR && checker != LF);

						stringFromChars(numStr, temp);
						typeVal = atof(numStr);

						if(type == 'Q') {
							active.x = -105.0 / SCALE_X;
							motor[extrude] = 0;
						}else if(type == 'X') active.x = typeVal / SCALE_X;
						else if(type == 'Y') active.y = typeVal / SCALE_Y;
						else if(type == 'E') active.e = typeVal;
/*
						if(active.e < 0.001) {
							motor[extrude] = 0;
						}
*/
					}while(checker != CR);

					writeDebugStreamLine("x: %f", active.x);
					writeDebugStreamLine("y: %f", active.y);
					writeDebugStreamLine("z: %f", active.z);
					writeDebugStreamLine("e: %f", active.e);

					ReadByte(parsingHandle, parsingIOResult, skipG);
					activeFull = true;
				}

				else {
					// TODO: End of File Routine
					// MAYBE A MODE SWITCH MAYBE?
				}
			}

			int xError = active.x - enc(xGan);
			int yError = active.y - enc(yGan);
			distance = (int)sqrt(xError * xError + yError * yError);

			if(distance > 20){
				xIVal += (ix * xError);
				yIVal += (iy * yError);

				if(xIVal > outMax) xIVal= outMax;
				else if(xIVal < outMin) xIVal = outMin;

				if(yIVal > outMax) yIVal= outMax;
				else if(yIVal < outMin) yIVal = outMin;

				float dXVal = (enc(xGan) - lastX);
				float dYVal = (enc(yGan) - lastY);

				float xOutput = px * xError + xIVal - dx * dXVal;
				float yOutput = py * yError + yIVal - dy * dYVal;

				if(xOutput > outMax) xOutput = outMax;
				else if(xOutput < outMin) xOutput = outMin;

				if(yOutput > outMax) yOutput = outMax;
				else if(yOutput < outMin) yOutput = outMin;
				float velocity = sqrt(2.7225 * xOutput * xOutput + 3.637 * yOutput * yOutput);
				motor(xGan) = xOutput * 120 / velocity;
				motor(yGan) = yOutput * 120 / velocity;

				/*Remember some variables for next time*/
				lastX = enc(xGan);
				lastY = enc(yGan);

				int thing = 60 - abs(xError) * 60 / 30.0;
				nxtSetPixel(counter, thing);
				counter++;
				} else {
				activeFull = false;
			}
			//writeDebugStreamLine("XError: %d", xError);
			//writeDebugStreamLine("YError: %d", yError);
			//writeDebugStreamLine("Distance: %d", distance);
			//if(activeFull)writeDebugStreamLine("ActiveFull: TRUE");
			//else writeDebugStreamLine("ActiveFull: FALSE");
			//if(nextFull)writeDebugStreamLine("NextFull: TRUE");
			//else writeDebugStreamLine("NextFull: FALSE");
			//writeDebugStreamLine("");
		}

		/********************************************************************************
		*																		MANUAL CONTROL
		********************************************************************************/
		if(mode == MANUAL) {
			getJoystickSettings(joystick);
			motor[xGan] = -joystick.joy1_x1;
			motor[yGan] = joystick.joy1_y1;

			if(joystick.joy1_Buttons == 128){ 	// extruder down
				motor[extrude] = extruderSpeed;
				} else if(joystick.joy1_Buttons == 32){	// extruder up
				motor[extrude] = -extruderSpeed;
				} else {
				motor[extrude] = 0;
			}

			if(joystick.joy1_Buttons == 2) { // zero encoders
				enc(xGan) = 0;
				enc(yGan) = 0;
			}

			if (prevStateRB && joystick.joy1_TopHat != 0){
				if(extruderSpeed < 100) {
					extruderSpeed++;
				}
			}

			if (prevStateRT && joystick.joy1_TopHat != 4){
				if(extruderSpeed > 0) {
					extruderSpeed--;
				}
			}

			prevStateRB = (joystick.joy1_TopHat == 0); // increase extruder speed boolean
			prevStateRT = (joystick.joy1_TopHat == 4); // decrease extruder speed boolean

		}

		lastButton = buttons;

		nxtDisplayString(0, "%d.%s", modeView, modeNames[modeView]);

		wait(50);
		eraseDisplay();
	}
}



void setXPID(float p, float i, float d) {
	px = p;
	ix = i;
	dx = d;
}

void setYPID(float p, float i, float d) {
	py = p;
	iy = i;
	dy = d;
}

void reset() {
	time1[T1] = 0;
	enc(yGan) = 0;
	enc(xGan) = 0;
}
