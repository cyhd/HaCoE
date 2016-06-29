#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawScatteringTheory : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawScatteringTheory();
	// Destructor
	~RemoteControlLawScatteringTheory();
	
	void compute();
	DataType send();

private :
	
	static const double F2N_K_PROPORTIONNAL;
	static const double F2N_K_INTEGRAL;
	static const double F2N_K_SCALE;

	Vector3 velocityIntegrator;
};