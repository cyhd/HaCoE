#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawSimple : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawSimple();
	// Destructor
	~RemoteControlLawSimple();
	
	Vector3 getForce( Vector3 transA, Vector3 transB);

private :
	Vector3 forceControlA;
	static const double F2N_K_FORCE;
	static const double F2N_SCALERFORCE;
};