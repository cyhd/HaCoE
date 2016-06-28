#include "RemoteControlLawDelayed.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawDelayed::RemoteControlLawDelayed(int timeDelay)
{
	this->setType(DELAYED_MODE);
	this->setTimeDelay(timeDelay);
	this->setDataNumber(1);
}

RemoteControlLawDelayed::~RemoteControlLawDelayed()
{

}

const double RemoteControlLawDelayed::F2N_K_PROPORTIONNAL = 0.002; //higher value makes the device vibrate
const double RemoteControlLawDelayed::F2N_K_INTEGRAL = 400; //higher value makes the device stiffer but can become unstable

void RemoteControlLawDelayed::compute()
{
	// the local data is delayed to compare local and remote velocity from the same time
	localVelocityDelayed = delay(localVelocity, LOCAL_VELOCITY);
	
	//classical velocity loop
	velocityIntegrator += (remoteVelocity-localVelocityDelayed)*F2N_K_INTEGRAL*fech;
	localAppliedForce = (velocityIntegrator + remoteVelocity-localVelocityDelayed)*F2N_K_PROPORTIONNAL;
}

DataType RemoteControlLawDelayed::send()
{
	sendDataType = LOCAL_VELOCITY;
	return sendDataType;
}