#include "OmniDevice.h"
using namespace std;


HDSchedulerHandle gSchedulerCallback = HD_INVALID_HANDLE;
HDErrorInfo lastError;

#define DEVICE_NAME	"PHANToM 1"

// SensAble device handle and scheduler declarations.
HHD phantomidLocal = HD_INVALID_HANDLE;  // Phantom declaration.

//init device data
hduVector3Dd OmniDevice::posLocal(0,0,0);
hduVector3Dd OmniDevice::forceLocal(0,0,0);
hduVector3Dd OmniDevice::velocityLocal(0,0,0);


OmniDevice::OmniDevice(int index)
{
	// some sensable error handling junk
    HDErrorInfo error;
	phantomidLocal = hdInitDevice(DEVICE_NAME);
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
	omniErrorCheck();


	/*//used to compensate the friction in case it's different from one to another device
	if (index == 0) //left device
	{
		frictionCompensation = hduVector3Dd(0.2,0.7,0.5);
	}
	else if (index == 1) //right device
	{
		frictionCompensation = hduVector3Dd(0.5,0.9,0.5);
	}*/
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
}

/** update the Omni instance (deviceA) from the data read by the Omni API callback
*/
short OmniDevice::readData()
{
	this->setTranslation(posLocal[0], posLocal[1], posLocal[2]);
	this->setVelocity(velocityLocal[0], velocityLocal[1], velocityLocal[2]);
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
	for (int i = 0; i < 3; i++)
		forceLocal[i]=force[i];
}

/** only used to get position from the remote Omni (deviceB)
*/
void OmniDevice::writePosition( Vector3 position , Matrix3x3 rotation ) 
{
}

void OmniDevice::writeDamping( Vector3 translation , Vector3 rotation ) 
{
}


/******************************************************************************
 Main callback.  Retrieves position from local device
 and sets forces for local device.
******************************************************************************/
HDCallbackCode HDCALLBACK OmniDevice::DataManaging(void *data)
{
  	//**************************************************//
	/* 	Obtain position of each device's end effector   */
	//**************************************************//
    hdBeginFrame(phantomidLocal);
	hdGetDoublev(HD_CURRENT_POSITION, posLocal);
	hdGetDoublev(HD_CURRENT_VELOCITY, velocityLocal);
	
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



int OmniDevice::omniErrorCheck()
{
	HDErrorInfo error;
    
	if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        hduPrintError(stderr, &error, "Error during main loop\n"); 

        if (error.errorCode == HD_WARM_MOTORS &&
            hdIsEnabled(HD_FORCE_OUTPUT))
        {
            fprintf(stderr, "\nTurning forces off till motors cool down...\n");
            hdDisable(HD_FORCE_OUTPUT);
        }

        if (hduIsSchedulerError(&error))
        {
            /* Just quit, since the user likely unplugged the device */
            fprintf(stderr, "A scheduler error has been detected.\n");
            return HD_CALLBACK_DONE;
        }
    }
    else if (!hdIsEnabled(HD_FORCE_OUTPUT))
    {
        fprintf(stderr, "\nTurning forces back on...\n");
        hdEnable(HD_FORCE_OUTPUT);   
    }
	
	return HD_CALLBACK_CONTINUE;
}
