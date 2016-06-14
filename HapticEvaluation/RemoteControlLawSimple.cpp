#include "RemoteControlLawSimple.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawSimple::RemoteControlLawSimple()
{
	this->setType(POSITION_MODE);
}

RemoteControlLawSimple::~RemoteControlLawSimple()
{

}

const double RemoteControlLawSimple::F2N_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double RemoteControlLawSimple::F2N_SCALERFORCE = 1; //SCALERFORCE is used to modify the stiffness difference between the two different haptic devices


void RemoteControlLawSimple::compute()
{
		localForce.x = -F2N_K_FORCE*( localPosition.x - remotePosition.x ); 
		localForce.y = -F2N_K_FORCE*( localPosition.y - remotePosition.y );
		localForce.z = -F2N_K_FORCE*( localPosition.z - remotePosition.z );
}