#pragma once

#include <qthread.h>
#include "HapticThread.h"

class WriteNetworkThread : public HapticThread
{
public:
	WriteNetworkThread( void );
	~WriteNetworkThread( void );
	void run();

private :
	Vector3 translation;
	char translationCom[3][8]; //to store the data for sending through the socket
};