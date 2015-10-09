#pragma once
#include "Device.h"


class OptiTrackDummy : public Device
{
public:
	OptiTrackDummy(void);
	~OptiTrackDummy(void);
	void closeConnection();

	short readData(); // changed by ME
};
