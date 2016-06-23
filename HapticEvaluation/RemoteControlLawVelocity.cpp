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
	localAppliedForce.x += F2N_K_PROPORTIONNAL*(remoteVelocity.x-localVelocity.x);
	localAppliedForce.y += F2N_K_PROPORTIONNAL*(remoteVelocity.y-localVelocity.y);
	localAppliedForce.z += F2N_K_PROPORTIONNAL*(remoteVelocity.z-localVelocity.z);
	
	/*
		localForce.x =  F2N_K_PROPORTIONNAL*((1+fech*F2N_K_INTEGRAL)*
			(remoteVelocity.x-localVelocity.x)-(remoteVelocityOld.x-localVelocityOld.x)); 
		localForce.y =  F2N_K_PROPORTIONNAL*((1+fech*F2N_K_INTEGRAL)*
			(remoteVelocity.y-localVelocity.y)-(remoteVelocityOld.y-localVelocityOld.y)); 
		localForce.z =  F2N_K_PROPORTIONNAL*((1+fech*F2N_K_INTEGRAL)*
			(remoteVelocity.z-localVelocity.z)-(remoteVelocityOld.z-localVelocityOld.z)); 

		remoteVelocityOld.x = remoteVelocity.x;
		remoteVelocityOld.y = remoteVelocity.y;
		remoteVelocityOld.z = remoteVelocity.z;

		localVelocityOld.x = localVelocity.x;
		localVelocityOld.y = localVelocity.y;
		localVelocityOld.z = localVelocity.z;
	*/
}

DataType RemoteControlLawVelocity::send()
{
	sendDataType = LOCAL_VELOCITY;
	return sendDataType;
}