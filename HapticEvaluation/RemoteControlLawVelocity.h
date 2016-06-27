#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawVelocity : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawVelocity();
	// Destructor
	~RemoteControlLawVelocity();

	void compute();
	DataType send();
	

private :
	static const double F2N_K_PROPORTIONNAL;
	static const double F2N_K_INTEGRAL;
	Vector3 velocityIntegrator;

};