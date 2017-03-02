#pragma once

#include "RemoteControlLaw.h"

class RemoteControlLawWaveTheory : public RemoteControlLaw
{
public : 
	// Constructor
	RemoteControlLawWaveTheory();
	// Destructor
	~RemoteControlLawWaveTheory();
	void compute();
	DataType send();

private :
	static const double F2N_K_PROPORTIONNAL;
	static const double F2N_K_INTEGRAL;
	static const double F2N_OPPOSITE_WAVE;

	Vector3 velocityIntegrator;
};