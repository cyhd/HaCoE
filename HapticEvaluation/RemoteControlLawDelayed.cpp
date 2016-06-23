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
	delayValue = timeDelay;
	if (delayValue == 0)
		delayValue = 1;
	for (int i = 0; i < delayValue; i++)
		localPositionBuff[i]=Vector3(0.0,0.0,0.0);
	cpt = 0;
	
}

RemoteControlLawDelayed::~RemoteControlLawDelayed()
{

}


Vector3 RemoteControlLawDelayed::delayData(Vector3 localPositionNew)
{
	localPositionBuff[cpt] = localPositionNew;
	cpt++;
	if(cpt == delayValue)
		cpt = 0;
	return localPositionBuff[cpt];
}


void RemoteControlLawDelayed::compute()
{
		localPositionDelayed = delayData(localPosition);	

		localAppliedForce.x = -F2N_K_FORCE*( localPositionDelayed.x - remotePosition.x ); 
		localAppliedForce.y = -F2N_K_FORCE*( localPositionDelayed.y - remotePosition.y );
		localAppliedForce.z = -F2N_K_FORCE*( localPositionDelayed.z - remotePosition.z );
}