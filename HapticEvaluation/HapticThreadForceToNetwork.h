#pragma once
#include <qthread.h>
#include "HapticThread.h"

class HapticThreadForceToNetwork : public HapticThread
{
public:
	HapticThreadForceToNetwork( void );
	~HapticThreadForceToNetwork( void );
	void run();

	void initUDPReadWrite(unsigned short portREAD, std::string ip, std::string portWRITE, int timeDelay);

private:
	Vector3 transA; //translational position of haptic device
	Vector3 transB;
	bool HapticThreadForceToNetworkStarted;

	QThread *threadRead;
	QThread *threadWrite;

	void initUDPWrite(std::string ip, std::string port, int timeDelay);
	void initUDPRead(unsigned short port);
	
};