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
	short readData( void ); // declared as virtual function in Device

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

	enum omniTeleoperationMode {NORMAL, NO_FORCE_FEEDBACK	};
	HDCallbackCode HDCALLBACK DeviceStateCallback(void *pUserData);
	void inititalizeDualOmnis();
	void shutdownDualOmnis();
	int omniErrorCheck();
	void MasterSlave();
	void exitHandler();
	static hduVector3Dd velocityEstimate(hduVector3Dd NewPosition, bool MasterorSlave);
	static HDCallbackCode HDCALLBACK MasterSlaveCallback(void *data);
	static void omniTeleoperationController(double virtualSpringConstant, double virtualDampingConstant, hduVector3Dd gravityCompensationVector, omniTeleoperationMode teleopMode = NORMAL);

	
protected:
	
private:

	double motionIn[3]; // array which the Omni reads.  The force and torque will be input to this array.
	double positionIn[3]; // for position control

	double damping[6]; //array of damping constants

	bool connectSuccess;

	
};