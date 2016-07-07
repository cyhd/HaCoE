#include "RemoteControlLawScatteringTheory.h"


RemoteControlLawScatteringTheory::RemoteControlLawScatteringTheory()
{
	this->setType(SCATTERING_MODE);
	sendDataType = LOCAL_APPLIED_FORCE;
	this->setDataNumber(3);

}

RemoteControlLawScatteringTheory::~RemoteControlLawScatteringTheory()
{

}

const double RemoteControlLawScatteringTheory::F2N_K_PROPORTIONNAL = 0.002; //higher value makes the device vibrate
const double RemoteControlLawScatteringTheory::F2N_K_INTEGRAL = 400; //higher value makes the device stiffer but can become unstable
const double RemoteControlLawScatteringTheory::F2N_K_SCALE = 1;

void RemoteControlLawScatteringTheory::compute()
{

	desiredLocalPosition = remoteVelocity + (remoteAppliedForce-localAppliedForce)*F2N_K_SCALE;

	velocityIntegrator += (desiredLocalPosition-localVelocity)*F2N_K_INTEGRAL*fech;
	localAppliedForce = (velocityIntegrator + desiredLocalPosition-localVelocity)*F2N_K_PROPORTIONNAL;

}

DataType RemoteControlLawScatteringTheory::send()
{
	switch(sendDataType)
		{
		case LOCAL_APPLIED_FORCE :
			sendDataType = LOCAL_VELOCITY;
			break;
		case LOCAL_VELOCITY :
			sendDataType = LOCAL_APPLIED_FORCE;
			break;
		}	

	return sendDataType;
}