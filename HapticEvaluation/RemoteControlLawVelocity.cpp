#include "RemoteControlLawVelocity.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawVelocity::RemoteControlLawVelocity()
{
	this->setType(VELOCITY_MODE);

	localVelocityOld = Vector3(0.0,0.0,0.0);
	remoteVelocityOld = Vector3(0.0,0.0,0.0);
}

RemoteControlLawVelocity::~RemoteControlLawVelocity()
{

}

const double RemoteControlLawVelocity::F2N_K_PROPORTIONNAL = 0.1; 
const double RemoteControlLawVelocity::F2N_K_INTEGRAL = 1; 

void RemoteControlLawVelocity::compute()
{
	localAppliedForce += (remoteVelocity-localVelocity)*F2N_K_PROPORTIONNAL;
	
}

DataType RemoteControlLawVelocity::send()
{
	sendDataType = LOCAL_VELOCITY;
	return sendDataType;
}