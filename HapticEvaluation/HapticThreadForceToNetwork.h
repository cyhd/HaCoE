#pragma once
#include <qthread.h>
#include "HapticThread.h"

class HapticThreadForceToNetwork : public HapticThread
{
public:
	HapticThreadForceToNetwork( void );
	~HapticThreadForceToNetwork( void );
	void run();
private:
	Vector3 transA; //translational position of haptic device
	Vector3 transB;
	
};