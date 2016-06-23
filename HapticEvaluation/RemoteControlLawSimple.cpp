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

const double RemoteControlLawSimple::F2N_K_FORCE = 0.04; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable

void RemoteControlLawSimple::compute()
{
		localAppliedForce.x = -F2N_K_FORCE*( localPosition.x - remotePosition.x ); 
		localAppliedForce.y = -F2N_K_FORCE*( localPosition.y - remotePosition.y );
		localAppliedForce.z = -F2N_K_FORCE*( localPosition.z - remotePosition.z );
}

DataType RemoteControlLawSimple::send()
{
	sendDataType = LOCAL_POSITION;
	return sendDataType;
}