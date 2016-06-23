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

private :
	static const double F2N_K_FORCE;
	static const double F2N_DAMPING;
};