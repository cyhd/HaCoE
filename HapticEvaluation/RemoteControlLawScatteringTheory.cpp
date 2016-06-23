#include "RemoteControlLawScatteringTheory.h"


RemoteControlLawScatteringTheory::RemoteControlLawScatteringTheory()
{
	this->setType(SCATTERING_MODE);
}

RemoteControlLawScatteringTheory::~RemoteControlLawScatteringTheory()
{

}

const double RemoteControlLawScatteringTheory::F2N_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double RemoteControlLawScatteringTheory::F2N_DAMPING = 0; //SCALERFORCE is used to modify the stiffness difference between the two different haptic devices

void RemoteControlLawScatteringTheory::compute()
{
	
	// Put this law to the master
	 
	localForce.x = remoteAppliedForce.x + F2N_K_FORCE*(localPosition.x - desiredRemotePosition.x) - F2N_DAMPING*localVelocity.x;
	localForce.y = remoteAppliedForce.x + F2N_K_FORCE*(localPosition.y - desiredRemotePosition.y) - F2N_DAMPING*localVelocity.y;
	localForce.z = remoteAppliedForce.x + F2N_K_FORCE*(localPosition.z - desiredRemotePosition.z) - F2N_DAMPING*localVelocity.z;
	
	localAppliedForce = saturation(localForce, 50);
	
	


	
	// Put this law to the slave
	/*
	desiredLocalPosition.x = remotePosition.x + 1/F2N_K_FORCE*(remoteAppliedForce.x - localAppliedForce.x);
	desiredLocalPosition.y = remotePosition.y + 1/F2N_K_FORCE*(remoteAppliedForce.y - localAppliedForce.y);
	desiredLocalPosition.z = remotePosition.z + 1/F2N_K_FORCE*(remoteAppliedForce.z - localAppliedForce.z);

	desiredLocalPosition = saturation(desiredLocalPosition, 150);

	localAppliedForce.x = F2N_K_FORCE*(desiredLocalPosition.x - localPosition.x);
	localAppliedForce.y = F2N_K_FORCE*(desiredLocalPosition.y - localPosition.y);
	localAppliedForce.z = F2N_K_FORCE*(desiredLocalPosition.z - localPosition.z);
	*/
	
	localAppliedForce = hysteresis(localAppliedForce, 1);
}
