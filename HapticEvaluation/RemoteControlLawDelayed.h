#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawDelayed : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawDelayed(int timeDelay);
	// Destructor
	~RemoteControlLawDelayed();
	Vector3 delayData(Vector3 localPositionNew);
	void compute();

private :
	double F2N_K_FORCE;
	int delayValue;
	int cpt;
	Vector3 localPositionBuff[1000];
	Vector3 localPositionDelayed;
};