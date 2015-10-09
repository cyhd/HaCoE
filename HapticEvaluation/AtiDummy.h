#pragma once
#include "Device.h"

class AtiDummy : public Device
{
public:
	AtiDummy(char *filename);
	~AtiDummy(void);
	void closeConnection();

    short readData(); // changed by me
};

