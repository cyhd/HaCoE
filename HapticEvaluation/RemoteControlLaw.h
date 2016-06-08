#pragma once
#include "datamodel.h"

/*****************************************************************************
Parent class for the control laws. 
*****************************************************************************/


class RemoteControlLaw
{
public :
	//empty Constructor
	RemoteControlLaw(){}
	~RemoteControlLaw(){}
	

	//virtual function that is implemented differently depending on the control law
	virtual Vector3 getForce(Vector3 transA, Vector3 transB) = 0;

private :
};

