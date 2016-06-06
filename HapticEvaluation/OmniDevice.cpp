#include "OmniDevice.h"
using namespace std;


HDSchedulerHandle gSchedulerCallback = HD_INVALID_HANDLE;
HDErrorInfo lastError;

#define DEVICE_NAME_LOCAL	"PHANToM 1"
#define DEVICE_NAME_REMOTE	"PHANToM 2"

// SensAble device handle and scheduler declarations.
HHD phantomidLocal = HD_INVALID_HANDLE;  // Phantom declaration.
HHD phantomidRemote = HD_INVALID_HANDLE;  // Phantom declaration.

//Scaling factor for gravity compensation to account for extra weight
float Scale_Master = 1.5;

struct DeviceDisplayStates
{
	HHD m_hHD;
	hduVector3Dd position;
	hduVector3Dd force;
};

OmniDevice::OmniDevice(int index)
{
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
		break;
	
	case 2:
		phantomidRemote = hdInitDevice(DEVICE_NAME_REMOTE);
		connectSuccess = true;
		//setMode(FORCECONTROL_MODE);
		setMode(VIRTUAL_MODE);
		break;
		
	}
}


OmniDevice::~OmniDevice()
{
	OmniDevice::closeConnection();
}

HDCallbackCode HDCALLBACK OmniDevice::DeviceStateCallback(void *pUserData)
{
	DeviceDisplayStates *pDisplayState = static_cast<DeviceDisplayStates *>(pUserData);
	
	hdMakeCurrentDevice(pDisplayState->m_hHD);
	hdGetDoublev(HD_CURRENT_POSITION, pDisplayState->position);
	hdGetDoublev(HD_CURRENT_FORCE, pDisplayState->force);

	return HD_CALLBACK_DONE;
}

void OmniDevice::closeConnection()
{
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

short OmniDevice::readData()
{
	hdScheduleAsynchronous(
	
	hdBeginFrame(phantomidLocal);
    	
	hdMakeCurrentDevice(phantomidLocal);
	hdGetDoublev(HD_CURRENT_POSITION, positiond);
	
	this->setTranslation(positiond[0], positiond[1], positiond[2]);
	positiond[0]=0.0;
	positiond[1]=0.0;
	positiond[2]=0.0;
	//hdEndFrame(phantomidLocal);
    
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
	
	//hdBeginFrame(phantomidLocal);
	hdMakeCurrentDevice(phantomidLocal);
	
	hdSetDoublev(HD_CURRENT_FORCE, forced);
	//hdEndFrame(phantomidLocal);
}

void OmniDevice::writePosition( Vector3 position , Matrix3x3 rotation ) 
{
	positiond[0]=position.x;
	positiond[1]=position.y;
	positiond[2]=position.z;
	//hdBeginFrame(phantomidRemote);
	hdMakeCurrentDevice(phantomidRemote);
	hdSetDoublev(HD_CURRENT_POSITION, positiond);
	//hdEndFrame(phantomidRemote);
	this->setTranslation(positiond[0], positiond[1], positiond[2]);
}

void OmniDevice::writeDamping( Vector3 translation , Vector3 rotation ) 
{
}
