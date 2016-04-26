#pragma once

#include <HD/hd.h>
#include <HDU/hduError.h>
#include <HDU/hduVector.h>
#include <conio.h>
#include <cstring>
#include <stdlib.h>
#include <iostream>
#include "Device.h"
#include "HapticDevice.h"


using namespace std;

class OmniDevice : public HapticDevice
{
public:
	// Constructor
	OmniDevice(void);

	// Destructor
	~OmniDevice(void);	
	void closeConnection( void ); //declared as virtual function in Device
	short readData( void *pUserData ); // declared as virtual function in Device

	//getters
	int getMode();
	bool getConnectSuccess() const { return connectSuccess; }

	//setters
	int setMode();
	void writeForce(Vector3 force );
	void writePosition(Vector3 position );
	void writeDamping(Vector3 translation , Vector3 rotation);

	//calibration
	int calibrate(void);

	//HDCallbackCode HDCALLBACK DeviceStateCallback(void *pUserData);
	int omniErrorCheck();
	void exitHandler();
	static HDCallbackCode HDCALLBACK MasterSlaveCallback(void *data);
	
protected:
	
private:

	double position[3]; // Store the position of the Omni device

	double motionIn[3]; // array which the Omni reads.  The force and torque will be input to this array.
	double positionIn[3]; // for position control

	double damping[6]; //array of damping constants

	bool connectSuccess;

	
};