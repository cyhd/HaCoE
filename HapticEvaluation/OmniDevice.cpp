#include "OmniDevice.h"
using namespace std;


HDSchedulerHandle gSchedulerCallback = HD_INVALID_HANDLE;
HDErrorInfo lastError;

#define DEVICE_NAME_MASTER	"PHANToM 2"
#define DEVICE_NAME_SLAVE	"PHANToM 1"

// Define Omni positions and velocities.
hduVector3Dd posMaster, posSlave;
hduVector3Dd velMaster, velSlave;

hduVector3Dd forceVec(0,0,0);
hduVector3Dd forceVec1(0,0,0);
hduVector3Dd forceVec2(0,0,0);
hduVector3Dd forceMaster(0,0,0);
hduVector3Dd forceSlave(0,0,0);


// SensAble device handle and scheduler declarations.
HHD phantomidMaster = HD_INVALID_HANDLE;  // Phantom on the left.
HHD phantomidSlave = HD_INVALID_HANDLE;	// Phantom on the right.
//Scaling factor for gravity compensation to account for extra weight
float Scale_Master = 1.5;

#define MASTER 1
#define SLAVE 0

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
    phantomidMaster = hdInitDevice(DEVICE_NAME_MASTER);

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
}


HDCallbackCode HDCALLBACK OmniDevice::DeviceStateCallback(void *pUserData)
{
    DeviceDisplayStates *pDisplayState = static_cast<DeviceDisplayStates *>(pUserData);

    hdMakeCurrentDevice(pDisplayState->m_hHD);      
    hdGetDoublev(HD_CURRENT_POSITION, pDisplayState->position);
    hdGetDoublev(HD_CURRENT_FORCE, pDisplayState->force);

    return HD_CALLBACK_DONE;
}

hduVector3Dd OmniDevice::velocityEstimate(hduVector3Dd NewPosition, bool MasterorSlave)
{
	double T = 0.001; // seconds
	// Filter the sample
	double fc = 10;   // Hz
	double lambda = 2*3.14159265*fc;
	double w = lambda*T/(1+lambda*T);

	/* Velocity estimate Code */
	static hduVector3Dd OldPositionM(0.0,0.0,0.0);
	static hduVector3Dd OldVelocityM(0.0,0.0,0.0);
	static hduVector3Dd OldPositionS(0.0,0.0,0.0);
	static hduVector3Dd OldVelocityS(0.0,0.0,0.0);

	if (MasterorSlave == MASTER)
	{
		// Calculate the current velocity, assuming 1 ms between samples.
		hduVector3Dd NewVelocity = (NewPosition - OldPositionM) / 0.001;
		NewVelocity = w * NewVelocity + (1-w) * OldVelocityM;

		// Set variables for next iteration
		OldVelocityM = NewVelocity;
		OldPositionM = NewPosition;
		
		return NewVelocity;
	}
	else
	{
		// Calculate the current velocity, assuming 1 ms between samples.
		hduVector3Dd NewVelocity = (NewPosition - OldPositionS) / 0.001;
		NewVelocity = w * NewVelocity + (1-w) * OldVelocityS;

		// Set variables for next iteration
		OldVelocityS = NewVelocity;
		OldPositionS = NewPosition;	
		
		return NewVelocity;
	}
	
}


void OmniDevice::closeConnection( void )
{
    hdStopScheduler();
    hdUnschedule(gSchedulerCallback);

	// Shutdown the Omni
    if (phantomidMaster != HD_INVALID_HANDLE)
    {
        hdDisableDevice(phantomidMaster);
        phantomidMaster = HD_INVALID_HANDLE;
    }

}

void OmniDevice::writeForce( Vector3 force )
{
	motionIn[0] = force.x;  // forces
	motionIn[1] = force.y;
	motionIn[2] = force.z;

	hdMakeCurrentDevice(phantomidMaster);
    hdSetDoublev(HD_CURRENT_FORCE, motionIn);
	hdEndFrame(phantomidMaster);
}

void OmniDevice::writePosition( Vector3 position )
{
	positionIn[0] = position.x;
	positionIn[1] = position.y;
	positionIn[2] = position.z;
	
	hdMakeCurrentDevice(phantomidMaster);
    hdSetDoublev(HD_CURRENT_POSITION, positionIn);
	hdEndFrame(phantomidMaster);
}

void OmniDevice::writeDamping( Vector3 translation , Vector3 rotation )
{
	
	damping[0] = translation.x;
	damping[1] = translation.y;
	damping[2] = translation.z;
	damping[3] = rotation.x;
	damping[4] = rotation.y;
	damping[5] = rotation.z;

	setDampingEAPI( getEntactHandle() , damping , 6 );
}

void OmniDevice::omniTeleoperationController(double virtualSpringConstant, double virtualDampingConstant, hduVector3Dd gravityCompensationVector, omniTeleoperationMode teleopMode) 
{
	//**************************************************//
	/* 	Obtain position of each device's end effector   */
	//**************************************************//
    hdBeginFrame(phantomidMaster);
    hdGetDoublev(HD_CURRENT_POSITION, posMaster);
	velMaster = velocityEstimate(posMaster, true);

    hdBeginFrame(phantomidSlave);
    hdGetDoublev(HD_CURRENT_POSITION, posSlave);
	velSlave = velocityEstimate(posSlave, false);
	forceVec =  virtualSpringConstant * (posMaster - posSlave) + virtualDampingConstant * (velMaster - velSlave);
 	if(teleopMode == NO_FORCE_FEEDBACK)
	{			
		forceMaster = (Scale_Master * gravityCompensationVector);
	}
	else{
		forceMaster = forceVec + Scale_Master * gravityCompensationVector;
	}

	forceSlave = forceVec + gravityCompensationVector;

	//********************************//
	/* 	 Output forces to each Omni   */
	//********************************//

	// Identify left haptic device.
	hdMakeCurrentDevice(phantomidMaster);
    hdSetDoublev(HD_CURRENT_FORCE, forceMaster);
    
	// Identify second haptic device
    hdMakeCurrentDevice(phantomidSlave);
    hdSetDoublev(HD_CURRENT_FORCE, forceSlave);

	// Flush forces on the first device.
    hdEndFrame(phantomidMaster);
	// Flush forces on the second device.
    hdEndFrame(phantomidSlave);
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

/******************************************************************************
 Main callback.  Retrieves position from both devices, calculates forces,
 and sets forces for both devices.
******************************************************************************/
HDCallbackCode HDCALLBACK OmniDevice::MasterSlaveCallback(void *data)
{
  	double virtualSpringConstant=0.2;
	double virtualDampingConstant=.0;
	omniTeleoperationMode teleopMode = NO_FORCE_FEEDBACK;
	hduVector3Dd gravityCompensationVector;
	gravityCompensationVector.set(0.0,0.5,0.0);
	omniTeleoperationController(virtualSpringConstant,virtualDampingConstant, gravityCompensationVector, teleopMode);
    return HD_CALLBACK_CONTINUE;
}


/******************************************************************************
 This handler gets called when the process is exiting.  Ensures that HDAPI is
 properly shutdown.
******************************************************************************/
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