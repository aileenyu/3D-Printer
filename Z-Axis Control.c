task main(){

  SensorType[sensorTouch] = sensorTouch;

  int layerHeight = 1300;

  int numWithinRange = 0;
	float pVal, iVal, dVal, error, errSum, dErr, lastErr;
	float pGain = 2;
	float	iGain = 0.0001;
	float dGain = 0.0001;
	nMotorEncoder[motorA] = 0;

	while (true) {
	  if (nNxtButtonPressed == 1){
	    motor[motorA] = -80;
	  }
	  if (nNxtButtonPressed == 2){
	    motor[motorA] = 80;
	  }
	  if (nNxtButtonPressed == -1){
	    motor[motorA] = 0;
	  }

	  numWithinRange = 0;
		nMotorEncoder[motorA] = 0;

	  if (SensorValue[S2] == 1){
			while (numWithinRange < 20){

			  error = layerHeight - nMotorEncoder[motorA];
			  errSum += (error*20.0/1000);
	      dErr = ( error - lastErr ) / (20.0/1000);

	    	pVal = pGain * error;
	    	iVal = iGain * errSum;
	      dVal = dGain * dErr;
				motor[MotorA] = pVal + iVal + dVal;
				writeDebugStreamLine("%f" , error);

				if (error <= 2 && error >= -2){
				 numWithinRange++;
				}

			  lastErr = error;

				wait1Msec(20);

			}
			motor[motorA] = 0;
		}
  }
}
