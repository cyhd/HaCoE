#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawPosition : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawPosition();
	// Destructor
	~RemoteControlLawPosition();

	void compute();
	DataType send();

private :
	static const double F2N_K_PROPORTIONNAL_VELOCITY;
	static const double F2N_K_PROPORTIONNAL_POSITION;
	static const double F2N_K_INTEGRAL;
	Vector3 velocityIntegrator;
};