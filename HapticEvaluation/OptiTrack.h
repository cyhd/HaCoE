#pragma once
#include "Device.h"
#include "ftconfig.h"
#include "NPTrackingTools.h"
#pragma comment(lib, "NPTrackingTools.lib")

class OptiTrack : public Device
{
public:
	OptiTrack(void);
	~OptiTrack(void);

	void cameraInit(char *project);
	void setPO(void);

    short readData(); // changed by me
	
	void closeConnection();

private:

	NPRESULT result;
	//long mFrameCounter;

	float yaw,pitch,roll;
	float x,y,z;
	float qx,qy,qz,qw;
	
};

