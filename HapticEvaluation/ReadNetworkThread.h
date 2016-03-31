#pragma once

#include "datamodel.h"
#include "HapticThread.h"

class ReadNetworkThread : public HapticThread
{
public:
	ReadNetworkThread( void );
	~ReadNetworkThread( void );
	void run();

private :
	Vector3 translation;
	char* packetID;
	char* nbBytes;
	char translationCom[3][8];
};