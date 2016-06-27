#include "RemoteControlLawScatteringTheory.h"


RemoteControlLawScatteringTheory::RemoteControlLawScatteringTheory()
{
	this->setType(SCATTERING_MODE);
}

RemoteControlLawScatteringTheory::~RemoteControlLawScatteringTheory()
{

}

const double RemoteControlLawScatteringTheory::F2N_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double RemoteControlLawScatteringTheory::F2N_DAMPING = 0; 

void RemoteControlLawScatteringTheory::compute()
{
	
	// Put this law to the master
	 
	localForce = remoteAppliedForce + (localPosition - desiredRemotePosition)*F2N_K_FORCE - localVelocity*F2N_DAMPING;
	
	localAppliedForce = saturation(localForce, 50);
	
	
	// Put this law to the slave
	/*
	desiredLocalPosition = remotePosition + (remoteAppliedForce - localAppliedForce)/F2N_K_FORCE;
	
	desiredLocalPosition = saturation(desiredLocalPosition, 150);

	localAppliedForce = (desiredLocalPosition - localPosition)*F2N_K_FORCE;
	*/
	
	localAppliedForce = hysteresis(localAppliedForce, 1);
}

DataType RemoteControlLawScatteringTheory::send()
{
	switch(sendDataType)
			{
			case LOCAL_POSITION :
				sendDataType = LOCAL_APPLIED_FORCE;
				break;
			case LOCAL_FORCE :
				sendDataType = LOCAL_APPLIED_FORCE;
				break;
			case DESIRED_LOCAL_POSITION :
				sendDataType = LOCAL_APPLIED_FORCE;
				break;
			case LOCAL_APPLIED_FORCE :
				sendDataType = LOCAL_POSITION;
				break;
			case LOCAL_VELOCITY :
				sendDataType = LOCAL_APPLIED_FORCE;
				break;
			}	
	return sendDataType;
}