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

/**********************************************************************************************************
HapticDevice class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class is used to make a direct communication with the haptic device. The class is virtual.
This class inherits from Device.

This class is NOT singleton. This is because there has to be instance of class constructed for each of 
the haptic devices (Entact W5D).  This class is based off the work of Boris Takac.
*************************************************************************************************************/
#pragma once

#include "datamodel.h"
#include "Device.h"

enum HapticMode {
 DISABLED_MODE	= 0x0000,
 FORCECONTROL_MODE = 0x0001,
 TORQUECONTROL_MODE	= 0x0002,
 POSITIONCONTROL_MODE = 0x0003,
 VIRTUAL_MODE = 0x0004
};

class HapticDevice : public Device
{
public:
	virtual int getMode() const { return mode; };
	virtual int setMode( HapticMode setmode ) { return mode = setmode; }
	virtual int calibrate() = 0;
	virtual void writeForce( Vector3 force , Vector3 torque ) = 0;
	virtual void writePosition( Vector3 position , Matrix3x3 rotation ) = 0;
	virtual void writeDamping( Vector3 translation , Vector3 rotation ) = 0;
	virtual bool getConnectSuccess() const = 0;
	//empty Constructor
	HapticDevice() : Device( HAPTIC_DEVICE ){}

protected:
	
private:
	Vector3 translation;
	Vector3 rotation;
	deviceType type;  
	int mode;
};

//#endif // DEVICE_H