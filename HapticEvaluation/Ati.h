#pragma once
#include "Device.h"
#include "ftconfig.h"

class Ati : public Device
{
public:
	Ati(void);
	~Ati(void);

    short readData(); // changed by me
	void closeConnection();

private:
	Calibration *cal;		// Struct containing calibration information	
	
	float voltages[6];
	float FT[6];	

	void nano17Init(char *filename);	

	void setVoltage(void);
   	void setFT(void);
};

