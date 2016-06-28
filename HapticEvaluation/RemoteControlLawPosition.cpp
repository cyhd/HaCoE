#include "RemoteControlLawPosition.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawPosition::RemoteControlLawPosition()
{
	this->setType(POSITION_MODE);
	this->setDataNumber(1);
}

RemoteControlLawPosition::~RemoteControlLawPosition()
{

}

const double RemoteControlLawPosition::F2N_K_PROPORTIONNAL_POSITION = 100; //higher value makes the device vibrate
const double RemoteControlLawPosition::F2N_K_PROPORTIONNAL_VELOCITY = 0.002; //higher value makes the device vibrate
const double RemoteControlLawPosition::F2N_K_INTEGRAL = 200; //higher value makes the device stiffer but can become unstable


void RemoteControlLawPosition::compute()
{
	desiredLocalPosition = (remotePosition - localPosition)*F2N_K_PROPORTIONNAL_POSITION;
	
	velocityIntegrator += (desiredLocalPosition-localVelocity)*F2N_K_INTEGRAL*fech;
	localAppliedForce = (velocityIntegrator + desiredLocalPosition-localVelocity)*F2N_K_PROPORTIONNAL_VELOCITY;
}

DataType RemoteControlLawPosition::send()
{
	sendDataType = LOCAL_POSITION;
	return sendDataType;
}