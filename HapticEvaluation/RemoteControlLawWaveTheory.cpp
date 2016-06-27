#include "RemoteControlLawWaveTheory.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawWaveTheory::RemoteControlLawWaveTheory(int timeDelay)
{
	this->setType(WAVE_MODE);
	delayValue = timeDelay;
	for (int i = 0; i < delayValue; i++)
		localForceDelayedBuff[i]=Vector3(0.0,0.0,0.0);
	cpt = 0;
	
	F2N_K_FORCE = 0.04; //K_FORCE is used to adjust the tightness of the control.  Higher values are more unstable
	F2N_DAMPING = 0; //The damping factor is used to slow down the waves
	F2N_OPPOSITE_WAVE = 1; //

}

RemoteControlLawWaveTheory::~RemoteControlLawWaveTheory()
{
}


Vector3 RemoteControlLawWaveTheory::delayData(Vector3 localForceNew)
{
	localForceDelayedBuff[cpt] = localForceNew;
	cpt++;
	if(cpt == delayValue)
		cpt = 0;
	return localForceDelayedBuff[cpt];
}


void RemoteControlLawWaveTheory::compute()
{
		localForce = ( localPosition - remotePosition )*(-F2N_K_FORCE);
		
		localForceDelayed = delayData(localForce);

		localAppliedForce = localForce - localForceDelayed*F2N_OPPOSITE_WAVE - localVelocity*F2N_DAMPING;
}

DataType RemoteControlLawWaveTheory::send()
{
	sendDataType = LOCAL_POSITION;
	return sendDataType;
}