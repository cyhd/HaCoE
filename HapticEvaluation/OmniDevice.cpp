#include "OmniDevice.h"
using namespace std;


HDSchedulerHandle gSchedulerCallback = HD_INVALID_HANDLE;
HDErrorInfo lastError;

#define DEVICE_NAME_MASTER	"PHANToM 1"

// SensAble device handle and scheduler declarations.
HHD phantomid = HD_INVALID_HANDLE;  // Phantom declaration.

//Scaling factor for gravity compensation to account for extra weight
float Scale_Master = 1.5;


OmniDevice::OmniDevice()
{
	// some sensable error handling junk
    HDErrorInfo error;

	// Device.
    phantomid = hdInitDevice(DEVICE_NAME_MASTER);

	if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        connectSuccess = true;
		printf("1. Found remote device: %s\n",hdGetString(HD_DEVICE_MODEL_TYPE));
	    setMode(VIRTUAL_MODE);
    }

	else
	{
		printf("1. Found device 1: %s\n",hdGetString(HD_DEVICE_MODEL_TYPE));
		hdEnable(HD_FORCE_OUTPUT);
		hdEnable(HD_FORCE_RAMPING);
		connectSuccess = true;
		setMode(FORCECONTROL_MODE);
	
		// Start the scheduler.
		hdStartScheduler();
		if (HD_DEVICE_ERROR(error = hdGetError()))
		{
			connectSuccess = false;
			hduPrintError(stderr, &error, "Failed to start scheduler");
			fprintf(stderr, "\nPress any key to quit.\n");
			getchar();
			exit(-1);
	}
	}
}


OmniDevice::~OmniDevice()
{
	OmniDevice::closeConnection();
}

void OmniDevice::closeConnection()
{
    hdStopScheduler();
    hdUnschedule(gSchedulerCallback);

	// Shutdown the Omni
    if (phantomid != HD_INVALID_HANDLE)
    {
        hdDisableDevice(phantomid);
        phantomid = HD_INVALID_HANDLE;
    }
}

short OmniDevice::readData()
{
	hdBeginFrame(phantomid);
    hdGetDoublev(HD_CURRENT_POSITION, positiond);
	hdEndFrame(phantomid);
	this->setTranslation(positiond[0], positiond[1], positiond[2]);
	
	return SUCCESS;
}



int OmniDevice::setMode(HapticMode setmode)
{
	mode = setmode;
	return mode;
}

void OmniDevice::writeForce(Vector3 force , Vector3 torque )
{
	forced[0]=force.x;
	forced[1]=force.y;
	forced[2]=force.z;
	
	hdBeginFrame(phantomid);
	hdSetDoublev(HD_CURRENT_FORCE, forced);
	hdEndFrame(phantomid);
}

void OmniDevice::writePosition( Vector3 position , Matrix3x3 rotation ) 
{
	positiond[0]=position.x;
	positiond[1]=position.y;
	positiond[2]=position.z;
	
	hdBeginFrame(phantomid);
	hdSetDoublev(HD_CURRENT_POSITION, positiond);
	hdEndFrame(phantomid);
	this->setTranslation(positiond[0], positiond[1], positiond[2]);
}

void OmniDevice::writeDamping( Vector3 translation , Vector3 rotation ) 
{
}
