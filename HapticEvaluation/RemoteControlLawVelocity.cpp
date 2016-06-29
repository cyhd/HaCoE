#include "RemoteControlLawVelocity.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawVelocity::RemoteControlLawVelocity()
{
	this->setType(VELOCITY_MODE);
	this->setDataNumber(1);
}

RemoteControlLawVelocity::~RemoteControlLawVelocity()
{

}

const double RemoteControlLawVelocity::F2N_K_PROPORTIONNAL = 0.002; //higher value makes the device vibrate
const double RemoteControlLawVelocity::F2N_K_INTEGRAL = 400; //higher value makes the device stiffer but can become unstable

void RemoteControlLawVelocity::compute()
{
	velocityIntegrator += (remoteVelocity-localVelocity)*F2N_K_INTEGRAL*fech;
	localAppliedForce = (velocityIntegrator + remoteVelocity-localVelocity)*F2N_K_PROPORTIONNAL;
}

DataType RemoteControlLawVelocity::send()
{
	sendDataType = LOCAL_VELOCITY;
	return sendDataType;
}