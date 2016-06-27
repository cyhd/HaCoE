#include "RemoteControlLawDelayed.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawDelayed::RemoteControlLawDelayed(int timeDelay)
{
	this->setType(DELAYED_MODE);
	F2N_K_FORCE = 0.04; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
}

RemoteControlLawDelayed::~RemoteControlLawDelayed()
{

}

void RemoteControlLawDelayed::compute()
{
		localPositionDelayed = delay(localPosition, LOCAL_POSITION);	

		localAppliedForce = (localPositionDelayed - remotePosition)*(-F2N_K_FORCE);
}

DataType RemoteControlLawDelayed::send()
{
	sendDataType = LOCAL_POSITION;
	return sendDataType;
}