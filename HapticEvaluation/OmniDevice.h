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
	OmniDevice();
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

	//calibration.  This will zero the device
	int calibrate() {return 0;}

		
protected:
	
private:
	bool connectSuccess; //see if connection was successful

	HapticMode mode;

	hduVector3Dd forced;
	hduVector3Dd positiond;
	hduVector3Dd translationd;
	hduVector3Dd rotationd;

	
};