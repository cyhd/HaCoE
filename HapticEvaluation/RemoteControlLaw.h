#pragma once
#include "datamodel.h"

class RemoteControlLaw
{
public :
	//empty Constructor
	RemoteControlLaw(){}
	~RemoteControlLaw(){}
	
	virtual Vector3 getForce(Vector3 transA, Vector3 transB) = 0;

private :
};

