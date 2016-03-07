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


	//getters

	//setters

	//calibration
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


	
};