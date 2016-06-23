#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawSimple : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawSimple();
	// Destructor
	~RemoteControlLawSimple();

	void compute();
	DataType send();

private :
	static const double F2N_K_FORCE;
};