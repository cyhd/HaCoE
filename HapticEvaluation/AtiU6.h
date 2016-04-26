//
// HaCoE Haptic Communication Evaluation is a software to build haptic communication experiments, using haptic arms and force sensors, generating standard XML files you can load in R.
//
//   Copyright (C) 2014 Cedric Dumas 
//
//   contributors: Cedric Dumas, Boris Takac, Devin Luu, Amine Chellali, Caroline Cao
//   contact: Cedric.Dumas@gmail.com
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//

/*****************************************************************************************
AtiU6 Class

This class contains methods for the LabJack U6 and ATI Nano17 force sensor device combo.
This class inherits from device.
*****************************************************************************************/

#pragma once
#include "Device.h"
#include "ftconfig.h"
#include "LabJackUD.h"
#pragma comment(lib, "labjackud.lib")

class AtiU6 : public Device
{
public:
	AtiU6(char *filename, char *serialNumber);
	~AtiU6(void);

    short readData(); // changed by me
	void closeConnection();

	bool getActivated() const {return activated;}
	void setActivated( bool status ){activated = status;}

	bool getConnectSuccess() const; // returns status of connection to LabJack.  This uses LabJack error codes instead of a member variable.

private:
	
	//Struct containing calibration information for Nano17	
	Calibration *cal;		
	float Total[6];
	//unsigned short index = 1;
	bool activated;
	//Labjack U6 variables
	LJ_ERROR lngErrorcode;
	LJ_HANDLE lngHandle;
	double dblValue;
	long lngIOType;
	long lngChannel;

	//Variables that store voltage from U6
	float voltages[6]; 
	//Variables that store Force & Torque
	float FT[6]; 

	LJ_ERROR nano17Init(char *filename, char *serialNumber); //initialize the labjack.  serialNumber is the serial number of the labjack.  filename is the file name of the calibration file of the ati connected to the labjack
	void setVoltage(void); // grab voltages from the labjack
   	void setFT(void); //convert voltages to forces/torques
};

