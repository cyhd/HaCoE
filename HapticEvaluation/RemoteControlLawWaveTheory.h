#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawWaveTheory : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawWaveTheory(int timeDelay);
	// Destructor
	~RemoteControlLawWaveTheory();
	Vector3 delayData(Vector3 localForceNew); 
	void compute();

private :
	double F2N_K_FORCE;
	double F2N_DAMPING;
	double F2N_OPPOSITE_WAVE;
	Vector3 localForceDelayedBuff[2000];
	Vector3 localForceDelayed;
	int cpt;
	int delayValue;
};