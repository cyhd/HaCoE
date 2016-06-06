#pragma once

#include "datamodel.h"
#include "Device.h"
#include "HapticDevice.h"

#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>
#include <conio.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>

using namespace std;

class OmniDevice : public HapticDevice
{
public:
	// Constructor
	OmniDevice(int index);
	// Destructor
	~OmniDevice();
	
	void closeConnection(void);//declared as virtual function in Device
	short readData(void);// declared as virtual function in Device

	//getters
	bool getConnectSuccess() const { return connectSuccess; }
	int getMode() const { return mode; }  //no member variable for this one.  Grab it directly from the entact.  getModeEAPI is from Entact API


	//setters
	int setMode( HapticMode setmode ) ; //sets the running mode of the Omni.  We should be in force mode mostly
	void writeForce( Vector3 force , Vector3 torque );  //sets the force to output from the Omni
	void writePosition( Vector3 position , Matrix3x3 rotation ); //sets the position to output to the Omni
	void writeDamping( Vector3 translation , Vector3 rotation ); //sets the damping to the Omni

	//Starts the scheduler that handle the data processing
	int calibrate();
	
protected:
	
private:
	//Function that manage the communication with the robot
	static HDCallbackCode HDCALLBACK DataManaging(void *data);
	
	//static hduVector3Dd posRemote;
	static hduVector3Dd forceLocal;
	static hduVector3Dd posLocal;
	
	/*
	hduVector3Dd posRemote2;
	hduVector3Dd forceLocal2;
	hduVector3Dd posLocal2;
	*/

	bool connectSuccess; //see if connection was successful

	HapticMode mode;
		
};