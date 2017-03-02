#pragma once
#include <qthread.h>
#include "HapticThread.h"

class HapticThreadForceToNetwork : public HapticThread
{
	Q_OBJECT
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
	int cpt;

	int timeStart;
	int currentTime;
	int TimeCpt;
	double time;

	void initUDPWrite(std::string ip, std::string port, int timeDelay);
	void initUDPRead(unsigned short port);

};


