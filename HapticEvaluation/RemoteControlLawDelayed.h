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
	static const double F2N_K_PROPORTIONNAL;
	static const double F2N_K_INTEGRAL;
	Vector3 velocityIntegrator;
};