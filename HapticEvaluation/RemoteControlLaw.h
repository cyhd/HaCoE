#pragma once
#include "datamodel.h"

/*****************************************************************************
Parent class for the control laws. 
*****************************************************************************/

enum ControlMode {
 POSITION_MODE	= 0x0000,
 SCATTERING_MODE = 0x0001,
 };

enum DataType {
	LOCAL_POSITION = 0x0000,
	REMOTE_POSITION = 0x0001,

	LOCAL_FORCE = 0x0002,
	REMOTE_FORCE = 0x0003,
	
	LOCAL_VELOCITY = 0x0004,
	REMOTE_VELOCITY = 0x0005,

	DESIRED_LOCAL_VELOCITY = 0x0006,
	DESIRED_REMOTE_VELOCITY = 0x0007,
};


class RemoteControlLaw
{
public :
	//empty Constructor
	RemoteControlLaw(){}
	~RemoteControlLaw(){}
	
	void setData(Vector3 data, DataType dataType);
	Vector3 getData(DataType dataType);

	//virtual functions that is implemented differently depending on the control law
	virtual void compute() = 0;

	//getter of the command type 
	ControlMode getType() {return commandType;}


protected :

	void setType(ControlMode type)
	{
		commandType=type;
	}

	Vector3 localPosition;
	Vector3 localForce;
	Vector3 localVelocity;
	Vector3 desiredLocalVelocity;

	Vector3 remotePosition;
	Vector3 remoteForce;
	Vector3 remoteVelocity;
	Vector3 desiredRemoteVelocity;

private :
	/*****************************************************
	The commandType changes the data used by the control 
	law. Used by the thread "HapticThreadForceToNetwork"
	to get the relevant data for the control law to
	work.
	Value changed in the constructor of the herited 
	class.
	*****************************************************/
	ControlMode commandType;
};

