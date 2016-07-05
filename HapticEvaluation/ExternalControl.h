#pragma once

#include "RemoteControlLaw.h"

class ExternalControl : public RemoteControlLaw
{
public : 
	// Constructor
	ExternalControl();
	// Destructor
	~ExternalControl();

	void compute();
	DataType send();
	
	void trajectory();

private :
	static const double F2N_K_PROPORTIONNAL;
	static const double F2N_K_INTEGRAL;

	double MAX_SPEED;
	double MAX_ACCELERATION;

	Vector3 positionIntegrator;

	int cpt;
	int choice;
	double t[16];
	Vector3 squareAngle[4];
	Vector3 referencePosition;
};