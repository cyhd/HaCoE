#include "OmniDevice.h"
using namespace std;


HDSchedulerHandle gSchedulerCallback = HD_INVALID_HANDLE;
HDErrorInfo lastError;

#define DEVICE_NAME	"PHANToM"

// SensAble device handle and scheduler declarations.
HHD phantomid = HD_INVALID_HANDLE;  // Phantom device.

//Scaling factor for gravity compensation to account for extra weight
float Scale_Master = 1.5;

/* Haptic device record. */
struct DeviceDisplayStates
{
    HHD m_hHD;
    hduVector3Dd position;
    hduVector3Dd force;
};


OmniDevice::OmniDevice()
{	 
	// some sensable error handling junk
    HDErrorInfo error;

	// Left device.
    phantomid = hdInitDevice(DEVICE_NAME);

	if (HD_DEVICE_ERROR(error = hdGetError()))
    {
        connectSuccess = false;
    }
	else
		connectSuccess = true;
	
    hdEnable(HD_FORCE_OUTPUT);
    hdEnable(HD_FORCE_RAMPING);
}
	

OmniDevice::~OmniDevice()
{
	OmniDevice::closeConnection();
	//OmniDevice::exitHandler();
}

/*
short OmniDevice::readData( void )
{
	hdMakeCurrentDevice(phantomid);
	hdGetDoublev(HD_CURRENT_POSITION, position);
	hdBeginFrame(phantomid)
	this->setTranslation( position[0] , position[1] , position[2] ); //these positions are given in millimetres.  Send back the translation info first
		
	return true;
}
*/

//HDCallbackCode HDCALLBACK OmniDevice::DeviceStateCallback(void *pUserData)
short OmniDevice::readData( void *pUserData )
{
    DeviceDisplayStates *pDisplayState = static_cast<DeviceDisplayStates *>(pUserData);

	hdMakeCurrentDevice(pDisplayState->m_hHD);      
    hdGetDoublev(HD_CURRENT_POSITION, pDisplayState->position);
    hdGetDoublev(HD_CURRENT_FORCE, pDisplayState->force);

	this->setTranslation( position[0] , position[1] , position[2] ); //these positions are given in millimetres.  Send back the translation info first
	
    return HD_CALLBACK_DONE;
}


void OmniDevice::closeConnection( void )
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

void OmniDevice::writeForce( Vector3 force )
{
	motionIn[0] = force.x;  // forces
	motionIn[1] = force.y;
	motionIn[2] = force.z;

	hdMakeCurrentDevice(phantomid);
    hdSetDoublev(HD_CURRENT_FORCE, motionIn);
	hdEndFrame(phantomid);
}

void OmniDevice::writePosition( Vector3 position )
{
	positionIn[0] = position.x;
	positionIn[1] = position.y;
	positionIn[2] = position.z;
	
	hdMakeCurrentDevice(phantomid);
    hdSetDoublev(HD_CURRENT_POSITION, positionIn);
	hdEndFrame(phantomid);
}

/*
void OmniDevice::omniTeleoperationController(double virtualSpringConstant, double virtualDampingConstant, hduVector3Dd gravityCompensationVector, omniTeleoperationMode teleopMode) 
{

    hdBeginFrame(phantomid);
    hdGetDoublev(HD_CURRENT_POSITION, position);
	
     



	// Identify left haptic device.
	hdMakeCurrentDevice(phantomid);
    hdSetDoublev(HD_CURRENT_FORCE, forceMaster);
    
	// Flush forces on the first device.
    hdEndFrame(phantomid);
}

*/
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


/******************************************************************************
 This handler gets called when the process is exiting.  Ensures that HDAPI is
 properly shutdown.
******************************************************************************/
/*
void OmniDevice::exitHandler()
{
   
   if (!lastError.errorCode)
    {
        hdStopScheduler();
        hdUnschedule(gSchedulerCallback);
    }

    if (phantomidMaster != HD_INVALID_HANDLE)
    {
        hdDisableDevice(phantomidMaster);
        phantomidMaster = HD_INVALID_HANDLE;
    }

    if (phantomidSlave != HD_INVALID_HANDLE)
    {
        hdDisableDevice(phantomidSlave);
		phantomidSlave = HD_INVALID_HANDLE;
    }
}
*/