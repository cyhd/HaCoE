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
	
	void setData(Vector3 data, DataType dataType);
	Vector3 getData(DataType dataType);
	void setSampleTime(int echTime);
	Vector3 saturation(Vector3 data, double saturationValue); // prevent the daata from going out of range
	Vector3 hysteresis(Vector3 data, double hysteresisValue); //create a hysteresis of value around 0 to avoid vibration

	//virtual functions that is implemented differently depending on the control law
	virtual void compute() = 0;

	//getter of the command type 
	ControlMode getType() {return commandType;}


protected :

	void setType(ControlMode type)
	{
		commandType=type;
	}
	Vector3 localAppliedForce;
	Vector3 remoteAppliedForce;

	Vector3 localPosition;
	Vector3 localForce;
	Vector3 localVelocity;
	Vector3 desiredLocalPosition;

	Vector3 remotePosition;
	Vector3 remoteForce;
	Vector3 remoteVelocity;
	Vector3 desiredRemotePosition;

	double fech;

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

