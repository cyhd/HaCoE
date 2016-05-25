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
        connectSuccess = false;
		hduPrintError(stderr, &error, "Failed to initialize left haptic device");
        fprintf(stderr, "Make sure the configuration \"%s\" exists\n", DEVICE_NAME_MASTER);
        fprintf(stderr, "\nPress any key to quit.\n");
        getchar();
        exit(-1);
    }
	else
	{
	printf("1. Found device 1: %s\n",hdGetString(HD_DEVICE_MODEL_TYPE));
    hdEnable(HD_FORCE_OUTPUT);
    hdEnable(HD_FORCE_RAMPING);
	connectSuccess = true;
	}
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
    hdGetDoublev(HD_CURRENT_POSITION, position);
	hdEndFrame(phantomid);
	this->setTranslation(position[0], position[1], position[2]);
	
	return SUCCESS;
}


int OmniDevice::calibrate()
{
	return 1;
}

void OmniDevice::writeForce(Vector3 force , Vector3 torque )
{
	hdBeginFrame(phantomid);
	hdSetDoublev(HD_CURRENT_FORCE, force);
	hdEndFrame(phantomid);
}

void OmniDevice::writePosition( Vector3 position , Matrix3x3 rotation ) 
{
	hdBeginFrame(phantomid);
	hdSetDoublev(HD_CURRENT_POSITION, position);
	hdEndFrame(phantomid);
	this->setTranslation(position[0], position[1], position[2]);
}

void OmniDevice::writeDamping( Vector3 translation , Vector3 rotation ) 
{
}
