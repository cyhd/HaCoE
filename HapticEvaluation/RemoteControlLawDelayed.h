#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawDelayed : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawDelayed(int timeDelay);
	// Destructor
	~RemoteControlLawDelayed();
	void compute();
	DataType send();

private :
	double F2N_K_FORCE;
};