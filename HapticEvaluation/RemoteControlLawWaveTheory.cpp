#include "RemoteControlLawWaveTheory.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawWaveTheory::RemoteControlLawWaveTheory()
{
	this->setType(WAVE_MODE);
	sendDataType = LOCAL_VELOCITY;
	this->setDataNumber(1);
	//this->setTimeDelay(delayValue*2);
}

RemoteControlLawWaveTheory::~RemoteControlLawWaveTheory()
{
}

const double RemoteControlLawWaveTheory::F2N_K_PROPORTIONNAL = 0.002; //higher value makes the device vibrate
const double RemoteControlLawWaveTheory::F2N_K_INTEGRAL = 400; //higher value makes the device stiffer but can become unstable
const double RemoteControlLawWaveTheory::F2N_OPPOSITE_WAVE = 0.1;

void RemoteControlLawWaveTheory::compute()
{
	velocityIntegrator += (remoteVelocity-localVelocity)*F2N_K_INTEGRAL*fech;
	localForce = (velocityIntegrator + remoteVelocity-localVelocity)*F2N_K_PROPORTIONNAL;
	
	localForceDelayed = delay(localForce, LOCAL_FORCE);
	
	localAppliedForce = localForce - localForceDelayed*F2N_OPPOSITE_WAVE;
}

DataType RemoteControlLawWaveTheory::send()
{
	sendDataType = LOCAL_VELOCITY;
	return sendDataType;
}