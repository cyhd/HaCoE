#include "RemoteControlLawScatteringTheory.h"


RemoteControlLawScatteringTheory::RemoteControlLawScatteringTheory()
{
	this->setType(SCATTERING_MODE);
}

RemoteControlLawScatteringTheory::~RemoteControlLawScatteringTheory()
{

}

const double RemoteControlLawScatteringTheory::F2N_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double RemoteControlLawScatteringTheory::F2N_SCALERFORCE = 1; //SCALERFORCE is used to modify the stiffness difference between the two different haptic devices

void RemoteControlLawScatteringTheory::compute()
{
	desiredLocalVelocity.x = remoteVelocity.x + 1/(F2N_SCALERFORCE*F2N_SCALERFORCE)*(remoteForce.x - localForce.x);
	desiredLocalVelocity.y = remoteVelocity.y + 1/(F2N_SCALERFORCE*F2N_SCALERFORCE)*(remoteForce.y - localForce.y);
	desiredLocalVelocity.z = remoteVelocity.z + 1/(F2N_SCALERFORCE*F2N_SCALERFORCE)*(remoteForce.z - localForce.z);

	localForce.x = remoteForce.x + F2N_SCALERFORCE*F2N_SCALERFORCE*(localVelocity.x - desiredRemoteVelocity.x);
	localForce.y = remoteForce.y + F2N_SCALERFORCE*F2N_SCALERFORCE*(localVelocity.y - desiredRemoteVelocity.y);
	localForce.z = remoteForce.z + F2N_SCALERFORCE*F2N_SCALERFORCE*(localVelocity.z - desiredRemoteVelocity.z);
}
