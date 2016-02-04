/*******************************************************************************   
	EntactAPI.h

	This header defines all the library functions available for interfacing with
	Entact haptic devices using ethernet.

	Currently this API is limited to Windows XP, Vista and 7

*******************************************************************************/

#ifndef _EntactAPI
#define _EntactAPI

#if defined(_MSC_VER)	// Microsoft compiler
	#ifdef ENTACTAPI_EXPORTS
		#define ENTACTAPI_API __declspec(dllexport)
	#else
		#define ENTACTAPI_API __declspec(dllimport)
	#endif
#elif defined(__GNUC__) // GNU compiler
	#define ENTACTAPI_API
#else
	#error define your compiler
#endif

/*************************************************************************** 
	Defines 
***************************************************************************/
#define EAPI_DISABLED_MODE			(0x0000)
#define EAPI_FORCECONTROL_MODE		(0x0001)
#define EAPI_TORQUECONTROL_MODE		(0x0002)
#define EAPI_POSITIONCONTROL_MODE	(0x0003)

#define EAPI_ERR					(-1)
#define EAPI_OK						(1)

typedef void* eapi_device_handle;

extern "C" 
{
	/*************************************************************************** 
		API Initialization Functions
	***************************************************************************/
	
	/*
	openEAPI()
		eapi_device_handle handles[],  
			an array of device handles, NULL if no devices are attached
		int size,
			size of the handles[] array which is passed in

			Used to automatically search the network for attached devices.
			Each found devices is assigned a device handle (eapi_device_handle).
			The ordering for the devices within the array is based on which devices
			reply back to the API the quickest (a network broadcast is used).

			Note: This function is best used when the device is configured for DHCP.
			If the device has a static IP address (the default setting) it is recommended
			to use the connectDeviceEAPI() function below listed below this function


	returns:
		n_devices,	
			if the function call is successfull n_devices contains the number of attached
			devices found. n_devices will be 0 of no devices are found but the API is 
			intialized succesfully
		EAPI_ERR,	
			if an error occurs initializing the API 
	*/
	ENTACTAPI_API int openEAPI(eapi_device_handle handles[], int size);

	/*
	connectDeviceEAPI()
		eapi_device_handle handles[],  
			an array of device handles
		int index,  
			where in the handles array to connect the device
			
			Used to connect a device which is configured for a static IP address.

			ex,

			eapi_device_handle Devices[16];

			connectDeviceEAPI(Devices,0,"192.168.1.144");
			connectDeviceEAPI(Devices,1,"192.168.1.145");

			setModeEAPI(Devices[0],EAPI_DISABLED_MODE); // accessing 1.144
			setModeEAPI(Devices[1],EAPI_DISABLED_MODE); // accessing 1.145
			
			

	returns:
		EAPI_OK,	
			if the device is found (at the specified IP address) and connection is successfull
		EAPI_ERR,	
			if the device cannot be found at the specified IP address. 
			(why does this commonly occur?)
	*/
	ENTACTAPI_API int connectDeviceEAPI(eapi_device_handle handles[], int index, char ip_address[]);

	/*
	closeEAPI()

	returns:
		EAPI_OK,
			upon successfull API close
		EAPI_ERR,
			if an error occurs during de-initialization, or the API
			was never initialized in the first place
	*/
	ENTACTAPI_API int closeEAPI();

	/*************************************************************************** 
		Device Management Functions
	***************************************************************************/
	/*
	setSampleRateEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		int frequency,
			frequency in Hz to set the real-time loop running on the device (by default 1000)
			it is recommended to keep the frequency below 5000 to avoid real-time over-runs in the device firmware
	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the frequency update is unsuccessfull 
	*/
	ENTACTAPI_API int setSampleRateEAPI(eapi_device_handle handle, int frequency);
	/*
	setModeEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		int mode,
			use the masks above to set the various modes of operation
			EAPI_FORCECONTROL_MODE, in this mode forces are commanded to the device, and world space positions are returned to the API
			EAPI_TORQUECONTROL_MODE, in this mode motor torques are commanded to the device, and motor shaft angles are returned to the API
			EAPI_DISABLED_MODE, devices start up mode, in this mode the motor drivers are disabled such that the device cannot produce forces/torques
			EAPI_POSITIONCONTROL_MODE, in this mode position is commanded to the device, and actual positions are returned
	returns:
		EAPI_OK,	
			if the function call is successfull at changing the devices mode 
		EAPI_ERR,	
			if the device mode change is unsuccessfull 
	*/
	ENTACTAPI_API int setModeEAPI(eapi_device_handle handle, int mode);
	/*
	getModeEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
	returns:
		EAPI_FORCECONTROL_MODE / EAPI_TORQUECONTROL_MODE / EAPI_DISABLED_MODE / EAPI_POSITIONCONTROL_MODE,	
			depending on which mode the device is currently set in
		EAPI_ERR,	
			if an error occurs polling the device for its mode
	*/
	ENTACTAPI_API int getModeEAPI(eapi_device_handle handle);
	/*
	getDeviceInfoEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		char *productname,
			char array to store the product name of the device.
			a w5d is "Entact W5D\n"
		char *name,
			char array to store the name of the device
		int size
			size of the passed in char arrays
	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if unsuccessfull 
	*/
	ENTACTAPI_API int getDeviceInfoEAPI(eapi_device_handle handle, char *productname, char *name, int size);
	// Currently un-implemented
	// Will be used for communicating safety fault conditions back to the API in real-time
	ENTACTAPI_API int setEventCallbackEAPI(eapi_device_handle handle, void (*cbFunc) (unsigned char eventArg));

	/*************************************************************************** 
		Calibration and Joint Angles
	***************************************************************************/
	/*
	homeDeviceEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device

			This function is used to calibrate the haptic device such that the world positon can
			be relayed back to the API. The haptic device must be moved to its calibration position
			when this function is called. Entry into the force control mode (EAPI_FORCECONTROL_MODE
			is restricted if the device is not first homed.

	returns:
		EAPI_OK,	
			if the function call is successfull at calibrating the device 
		EAPI_ERR,	
			if the device fails to home (probably a communication error in UDP)
	*/
	ENTACTAPI_API int homeDeviceEAPI(eapi_device_handle handle);
	/*
	isHomedEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
	returns:
		,	
			if the function call is successfull n_devices contains the number of attached
			devices found. n_devices will be 0 of no devices are found but the API is 
			intialized succesfully
		EAPI_ERR,	
			if an error occurs 
	*/
	ENTACTAPI_API int isHomedEAPI(eapi_device_handle handle);
	/*
	NOTE: this function still needs some small additions to work properly

	readJointsEAPI()  
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		double *pos,
			an array to store the position values of each encoder of the haptic device.
			for the w5d there are 8 encoders which can be checked for position
		double *vel,
			an array to store the velocity values of each encoder of the haptic device.
		int size,
			the size of the pos and vel arrays. Note: these should both be the same length.
			for the w5d the pos and vel arrays should be 8 elements long

			Call this function to poll the device for its encoder position and velocity values.
			These are the position of each of the joints, and do NOT correspond to the world position/velocity.

			NOTE: you must call writeTorqueEAPI() in torque control mode to refresh the API's values for encoder position/velocity

	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the device fails to return the latest encoder values (probably a communication error in UDP)
	*/
	ENTACTAPI_API int readJointsEAPI(eapi_device_handle handle, double *pos, double *vel, int size);

	/*************************************************************************** 
		Task Position and Velocity and Jacobian
	***************************************************************************/
	/*
	readTaskPositionEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		double *taskpos,
			each element of this array will be filled with position/orientation information (in world space)
			for the w5d:
			taskpos[0] = X position (mm)
			taskpos[1] = Y position (mm)
			taskpos[2] = Z position (mm)
			taskpos[3] = Rotation Matrix Element 1,1
			taskpos[4] = Rotation Matrix Element 1,2
			taskpos[5] = Rotation Matrix Element 1,3
			taskpos[6] = Rotation Matrix Element 2,1
			taskpos[7] = Rotation Matrix Element 2,2
			taskpos[8] = Rotation Matrix Element 2,3
			taskpos[9] = Rotation Matrix Element 3,1
			taskpos[10] = Rotation Matrix Element 3,2
			taskpos[11] = Rotation Matrix Element 3,3
		int size,
			size of the taskpos array which is passed in to the function. for the w5d this would be 12

		This function is called to obtain the current position/orientation of the haptic devices handle.
		The device must be calibrated, and in force control mode.
		The values for position/orientation are updated every time a force is commanded to the device.

	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the device fails to retrieve the current positions
	*/
	ENTACTAPI_API int readTaskPositionEAPI(eapi_device_handle handle, double *taskpos, int size);
	/*
	readTaskVelocityEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		double *taskvel,
			each element of this array will be filled with velocity information (in world space)
			for the w5d:
			taskvel[0] = X velocity (mm/s)
			taskvel[1] = Y velocity (mm/s)
			taskvel[2] = Z velocity (mm/s)
			taskvel[3] = X angular velocity (rad/s)
			taskvel[4] = Y angular velocity (rad/s)
			taskvel[5] = Z angular velocity (rad/s)
		int size,
			size of the taskvel array which is passed in to the function.
			for the w5d this would be 6.

		The usage of this function is analogous to the Position function above.

	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the device fails to retrieve the current velocities
	*/
	ENTACTAPI_API int readTaskVelocityEAPI(eapi_device_handle handle, double *taskvel, int size);
	
	/*************************************************************************** 
		Force and Joint Torque
	***************************************************************************/
	/*
	writeForceEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		double *f,
			force command for the device.
			for a w5d:
			f[0] = X force (N)
			f[1] = Y force (N)
			f[2] = Z force (N)
			f[3] = X torque (N.mm)
			f[4] = Y torque (N.mm)
			f[5] = Z torque (N.mm)
		int size,
			size of the f array which is passed in to the function. for the w5d this would be 6 elements

			Call this function to command a force/torque vector to the handle of the device. 
			Note: this function may only be called in force control mode (after the device is calibrated)
			Also Note: In force control mode calling this function causes the API to refresh the current
			position/velocity of the device.

	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the device fails to update the devices force/torque setpoint
	*/
	ENTACTAPI_API int writeForceEAPI(eapi_device_handle handle, double *f, int size);

	/*
	writeTOrqueEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		double *tau,
			force command for the device.
			for a w5d:
			tau[0] = M1 torque (N.mm)
			tau[1] = M2 torque (N.mm)
			tau[2] = M3 torque (N.mm)
			tau[3] = M4 torque (N.mm)
			tau[4] = M5 torque (N.mm)
			tau[5] = M6 torque (N.mm)
		int size,
			size of the tau array which is passed in to the function. for the w5d this would be 6 elements

			Call this function to command a torque vector to the motors of the device. 
			Note: this function may only be called in torque control mode
			Also Note: In torque control mode calling this function causes the API to refresh the current
			joint position/velocity values of the device.

	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the device fails to update the devices torque setpoint
	*/
	ENTACTAPI_API int writeTorqueEAPI(eapi_device_handle handle, double *tau, int size);
	/*
	writePositionEAPI()
		eapi_device_handle handle,  
			an element of the device handles array, example: handle[0] for the first attached device
		double *pos,
			position command for the device.
			for a w5d:
			pos[0] = X
			pos[1] = Y
			pos[2] = Z
			pos[3] = Rotation Matrix Element 1,1
			pos[4] = Rotation Matrix Element 1,2
			pos[5] = Rotation Matrix Element 1,3
			pos[6] = Rotation Matrix Element 2,1
			pos[7] = Rotation Matrix Element 2,2
			pos[8] = Rotation Matrix Element 2,3
			pos[9] = Rotation Matrix Element 3,1
			pos[10] = Rotation Matrix Element 3,2
			pos[11] = Rotation Matrix Element 3,3
		int size,
			size of the f array which is passed in to the function. for the w5d this would be 12 elements

			Call this function to command a position and orientation to the handle of the device. 
			Note: this function may only be called in position control mode (after the device is calibrated)
			Also Note: In position control mode calling this function causes the API to refresh the actual
			position/velocity values of the device.

	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the device fails to update the devices position setpoint
	*/
	ENTACTAPI_API int writePositionEAPI(eapi_device_handle handle, double *pos, int size);
	/*************************************************************************** 
		Built in Haptic Effects
	***************************************************************************/
	/*
	setDampingEAPI()
		double *d,
			an array of damping constants.
			for the w5d:
			d[0] = damping in x
			d[1] = damping in y
			d[2] = damping in z
			d[3] = damping for x torque
			d[4] = damping for y torque
			d[5] = damping for z torque

		int size,
			size of the passed in double array, for the w5d this should be 6
		
		Used to implement the haptic devices internal damping capability. 
		Call this function after you have enabled the device.

	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the device fails to update the devices in built damping field
	*/
	ENTACTAPI_API int setDampingEAPI(eapi_device_handle handle, double *d, int size);
	/*
	setVibrationEAPI()
		int frequency,
			the frequency in Hz of the vibration

		double *f,
			an array of damping constants.
			for the w5d:
			fvibe[0] = vibe magnitude in x
			fvibe[1] = vibe magnitude in y
			fvibe[2] = vibe magnitude in z
			fvibe[3] = vibe magnitude in taux
			fvibe[4] = vibe magnitude in tauy
			fvibe[5] = vibe magnitude in tauz

		int size,
			size of the passed in double array, for the w5d this should be 6
		
		Used to implement the haptic devices internal vibration capability. 
		Call this function after you have enabled the device.

	returns:
		EAPI_OK,	
			if the function call is successfull 
		EAPI_ERR,	
			if the device fails to set the vibration
	*/
	ENTACTAPI_API int setVibrationEAPI(eapi_device_handle handle, int frequency, double *fvibe, int size);

};

#endif // _EntactAPI
