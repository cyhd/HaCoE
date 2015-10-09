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
EntactDevice class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class is used to make a direct communication with the haptic device. The class contains methods for 
Connecting and Disconnecting to haptic device. Data format of private memeber arrays in this class 
(i.e. position[]) is compliant with the data format used in device API (EntactAPI.h). Except arrays for 
dynamic data, this class also holds information specific to each individual device, like IP address or 
values of some coefficients.  This class inherits from HapticDevice.

This class is NOT singleton. This is because there has to be instance of class constructed for each of 
the haptic devices (Entact W5D).  This class is based off the work of Boris Takac.  
*************************************************************************************************************/
#pragma once

#include "datamodel.h"
#include "EntactAPI.h"
#include "Device.h"
#include "HapticDevice.h"

class EntactDevice : public HapticDevice
{
	static eapi_device_handle handle[2];

public:
	//Constructor
	EntactDevice( int index , char *ip_address );
	
	//Destructor
	~EntactDevice( void );
	void closeConnection( void ); //declared as virtual function in Device
	short readData( void ); // declared as virtual function in Device

	//getters
	char *getEntactIP() const { return entactIP; }
	int getMode() const { return getModeEAPI( getEntactHandle() ); } //no member variable for this one.  Grab it directly from the entact.  getModeEAPI is from Entact API
	int getIndex() const { return entactIndex; }
	eapi_device_handle getEntactHandle() const { return handle[getIndex()]; }
	bool getConnectSuccess() const { return connectSuccess; }

	//setters
	int setMode( int setmode ); //sets the running mode of the Entact.  We should be in force mode mostly
	void writeForce( Vector3 force , Vector3 torque );  //sets the force to output from the Entact
	void writePosition( Vector3 position , Matrix3x3 rotation ); //sets the position to output to the entact
	void writeDamping( Vector3 translation , Vector3 rotation ); //sets the damping to the Entact

	//calibration.  This will zero the device
	int calibrate( void );

private:
	
	//Variables fed to the HaptDevice to connect are stored here
	//They should never be changed
	char *entactIP;
	int entactIndex;
	
	double position[12]; // this array stores the position and angle of the entact.  See entactapi.h 
	//double velocity[6]; // uncomment to read velocities

	double motionIn[6]; // array which the Entact reads.  The force and torque will be input to this array.
	double positionIn[12]; // for position control

	double damping[6]; //array of damping constants
		
	//setters for connection and mode should only be used within the object
	//They change the variables that should never be changed.
	bool connectSuccess; //see if connection was successful
};