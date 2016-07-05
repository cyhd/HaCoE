#pragma once
#include <qthread.h>
#include "HapticThread.h"

class HapticThreadForceToNetwork : public HapticThread
{
public:
	HapticThreadForceToNetwork( bool mode );
	~HapticThreadForceToNetwork( void );
	void run();

	void initUDPReadWrite(unsigned short portREAD, std::string ip, std::string portWRITE, int timeDelay);

private:
	Vector3 position; //translational position of haptic device
	Vector3 velocity;
	bool HapticThreadForceToNetworkStarted;

	QThread *threadRead;
	QThread *threadWrite;

	bool testMode;

	void initUDPWrite(std::string ip, std::string port, int timeDelay);
	void initUDPRead(unsigned short port);
	
};