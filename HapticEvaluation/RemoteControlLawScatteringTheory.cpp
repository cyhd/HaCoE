#include "RemoteControlLawScatteringTheory.h"


RemoteControlLawScatteringTheory::RemoteControlLawScatteringTheory()
{
	this->setType(SCATTERING_MODE);
	sendDataType = LOCAL_APPLIED_FORCE;
}

RemoteControlLawScatteringTheory::~RemoteControlLawScatteringTheory()
{

}

const double RemoteControlLawScatteringTheory::F2N_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double RemoteControlLawScatteringTheory::F2N_DAMPING = 0; 

const double RemoteControlLawScatteringTheory::F2N_K_PROPORTIONNAL = 0.002; //higher value makes the device vibrate
const double RemoteControlLawScatteringTheory::F2N_K_INTEGRAL = 200; //higher value makes the device stiffer but can become unstable

void RemoteControlLawScatteringTheory::compute()
{
	
	velocityIntegrator += (remoteVelocity-localVelocity)*F2N_K_INTEGRAL*fech;
	localForce = (velocityIntegrator + remoteVelocity-localVelocity)*F2N_K_PROPORTIONNAL;
	 
	localAppliedForce = remoteForce + (localVelocity - desiredRemotePosition)*F2N_K_PROPORTIONNAL;
	desiredLocalPosition = remoteVelocity + (remoteAppliedForce-localForce)/F2N_K_PROPORTIONNAL;
	
}

DataType RemoteControlLawScatteringTheory::send()
{
	switch(sendDataType)
			{
			case LOCAL_APPLIED_FORCE :
				sendDataType = DESIRED_LOCAL_POSITION;
				break;
			case DESIRED_LOCAL_POSITION :
				sendDataType = LOCAL_VELOCITY;
				break;
			case LOCAL_VELOCITY :
				sendDataType = LOCAL_FORCE;
				break;
			case LOCAL_FORCE :
				sendDataType = LOCAL_APPLIED_FORCE;
				break;
			}	
	return sendDataType;
}