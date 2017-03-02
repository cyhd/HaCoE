#pragma once
#include "datamodel.h"

/*****************************************************************************
Parent class for the control laws. 
*****************************************************************************/


class RemoteControlLaw
{
public :
	//empty Constructor
	RemoteControlLaw();
	~RemoteControlLaw();
	
	void setData(Vector3 data, DataType dataType);
	Vector3 getData(DataType dataType);
	int getDataNumber(){return dataNumber;}
	void setSampleTime(int echTime);
	void setTimeDelay (int timeDelay);
	void setIndex(int index){this->index = index;}
	//particular methods used in some control laws
	Vector3 saturation(Vector3 data, double saturationValue); // prevent the daata from going out of range
	Vector3 hysteresis(Vector3 data, double hysteresisValue); //create a hysteresis of value around 0 to avoid vibration

	//virtual functions that is implemented differently depending on the control law
	virtual void compute() = 0;
	virtual DataType send() = 0;

	

protected :

	void setType(ControlMode type) {commandType=type;}
	void setDataNumber(int number) {dataNumber = number;}
	//current data of the robot or received from the UDP communication
	Vector3 localAppliedForce;
	Vector3 localPosition;
	Vector3 localForce;
	Vector3 localVelocity;
	Vector3 desiredLocalPosition;

	Vector3 remoteAppliedForce;
	Vector3 remotePosition;
	Vector3 remoteForce;
	Vector3 remoteVelocity;
	Vector3 desiredRemotePosition;

	//delay managing
	Vector3 delay(Vector3 data, DataType type); //method used to delay the data
	Vector3 delayedDataBuff[10][2000];
	int delayBuffCpt[10];
	int delayValue; //Used to delay data 
	int dataNumber;

	//delayed Data
	Vector3 localAppliedForceDelayed;
	Vector3 localPositionDelayed;
	Vector3 localForceDelayed;
	Vector3 localVelocityDelayed;
	Vector3 desiredLocalPositionDelayed;

	Vector3 remoteAppliedForceDelayed;
	Vector3 remotePositionDelayed;
	Vector3 remoteForceDelayed;
	Vector3 remoteVelocityDelayed;
	Vector3 desiredRemotePositionDelayed;


	double fech; //sample time, can be used for sample control theory 
	int index;//define the master/slave
	DataType sendDataType; //used to manage the data sent by WriteNetworkThread

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

