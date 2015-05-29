task main(){
	SensorType[sensorTouch] = sensorTouch;

  int layerHeight = 200;
  int numWithinRange = 0;

	float pVal, iVal, dVal, error, errSum, dErr, lastErr;
	float pGain = 2;
	float	iGain = 0.03;
	float dGain = 0.12;

	nMotorEncoder[motorA] = 0;

	while (true) {
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
