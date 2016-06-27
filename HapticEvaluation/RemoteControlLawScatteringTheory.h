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
	
	static const double F2N_K_FORCE;
	static const double F2N_DAMPING;
	static const double F2N_K_PROPORTIONNAL;
	static const double F2N_K_INTEGRAL;

	Vector3 velocityIntegrator;
};