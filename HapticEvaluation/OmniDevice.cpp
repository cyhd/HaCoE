#include "OmniDevice.h"
using namespace std;


HDSchedulerHandle gSchedulerCallback = HD_INVALID_HANDLE;
HDErrorInfo lastError;

#define DEVICE_NAME_LOCAL	"PHANToM 1"
#define DEVICE_NAME_REMOTE	"PHANToM 2"

// SensAble device handle and scheduler declarations.
HHD phantomidLocal = HD_INVALID_HANDLE;  // Phantom declaration.
HHD phantomidRemote = HD_INVALID_HANDLE;  // Phantom declaration.

//init device data

hduVector3Dd OmniDevice::posLocal(0,0,0);
hduVector3Dd OmniDevice::forceLocal(0,0,0);


//Scaling factor for gravity compensation to account for extra weight
float Scale_Master = 1.5;

/*
struct DeviceDisplayStates
{
	HHD m_hHD;
	hduVector3Dd position;
	hduVector3Dd force;
};*/


OmniDevice::OmniDevice(int index)
{
	/*
	posRemote = &posRemote2;
	forceLocal = &forceLocal2;
	posLocal = &posLocal2;
	*/
	// some sensable error handling junk
    HDErrorInfo error;

	switch (index)
	{
	case 1:
		phantomidLocal = hdInitDevice(DEVICE_NAME_LOCAL);
		hdEnable(HD_FORCE_OUTPUT);
		hdEnable(HD_FORCE_RAMPING);
		connectSuccess = true;
		setMode(FORCECONTROL_MODE);

		hdStartScheduler();
		if (HD_DEVICE_ERROR(error = hdGetError()))
		{
			hduPrintError(stderr, &error, "Failed to start scheduler");
        		fprintf(stderr, "\nPress any key to quit.\n");
        		getchar();
        		exit(-1);
		}

		gSchedulerCallback = hdScheduleAsynchronous(
			DataManaging, 0, HD_DEFAULT_SCHEDULER_PRIORITY);
		break;
	
	case 2:
		phantomidRemote = hdInitDevice(DEVICE_NAME_REMOTE);
		connectSuccess = true;
		//setMode(FORCECONTROL_MODE);
		setMode(VIRTUAL_MODE);
		break;
	}

	this->setTranslation(posLocal[0],posLocal[1],posLocal[2]);

}


OmniDevice::~OmniDevice()
{
	OmniDevice::closeConnection();
}

void OmniDevice::closeConnection()
{
    //Stop the scheduler
	hdStopScheduler();
    hdUnschedule(gSchedulerCallback);
		
	// Shutdown the Omni
    if (phantomidLocal != HD_INVALID_HANDLE)
    {
        hdDisableDevice(phantomidLocal);
        phantomidLocal = HD_INVALID_HANDLE;
    }
	if (phantomidRemote != HD_INVALID_HANDLE)
    {
        hdDisableDevice(phantomidRemote);
        phantomidRemote = HD_INVALID_HANDLE;
    }
}

/** update the Omni instance (deviceA) from the data read by the Omni API callback
*/
short OmniDevice::readData()
{
		
	this->setTranslation(posLocal[0], posLocal[1], posLocal[2]);
	
	return SUCCESS;
}



int OmniDevice::setMode(HapticMode setmode)
{
	mode = setmode;
	return mode;
}

/** used by the control law to update the device force
*/
void OmniDevice::writeForce(Vector3 force , Vector3 torque )
{
	forceLocal[0]=force.x;
	forceLocal[1]=force.y;
	forceLocal[2]=force.z;	
}

/** only used to get position from the remote Omni (deviceB)
*/
void OmniDevice::writePosition( Vector3 position , Matrix3x3 rotation ) 
{
	this->setTranslation(position.x, position.y, position.z);
}

void OmniDevice::writeDamping( Vector3 translation , Vector3 rotation ) 
{
}

HDCallbackCode HDCALLBACK OmniDevice::DataManaging(void *data)
{
  	//**************************************************//
	/* 	Obtain position of each device's end effector   */
	//**************************************************//
    hdBeginFrame(phantomidLocal);
	hdGetDoublev(HD_CURRENT_POSITION, posLocal);
	
	//********************************//
	/* 	 Output forces to each Omni   */
	//********************************//

	// Identify left haptic device.
	hdMakeCurrentDevice(phantomidLocal);
    hdSetDoublev(HD_CURRENT_FORCE, forceLocal);
    
	// Flush forces on the first device.
    hdEndFrame(phantomidLocal);
	
	return HD_CALLBACK_CONTINUE;
}

int OmniDevice::calibrate()
{
	
	
	
	return 1;
}   