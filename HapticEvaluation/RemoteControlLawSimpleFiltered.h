#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawSimpleFiltered : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawSimpleFiltered();
	// Destructor
	~RemoteControlLawSimpleFiltered();
	
	Vector3 getForce( Vector3 transA, Vector3 transB);

private :
	Vector3 forceControl[3];
	Vector3 forceControlA;
	int cpt;
	int size;
	static const double F2N_K_FORCE;
	static const double F2N_SCALERFORCE;
};