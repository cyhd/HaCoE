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
HapticThread class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class is used to control the haptic devices.  After the user chooses which haptic devices to activate
and presses start on the GUI, this thread will run.  This thread does not instantiate the haptic devices.  It 
only manages the control aspects (gathering sensor information, calculation of force vectors, and commanding 
motion of the haptic devices).  The main functionality of the thread is implemented in the run function.

The thread will poll the GUI through haptlinksupervisor for the stop command while running.  If a stop command
is detected, then the thread stops.
*************************************************************************************************************/
#pragma once
#include <qthread.h>
#include "datamodel.h"
#include "HapticDevice.h"

class HapticThread : public QThread
{
public:
	HapticThread(void){ zero.x=0; zero.y=0; zero.z=0;};
	virtual void run() = 0;  //main function of the thread
	~HapticThread(void){};

protected:
	//Vectors for controlling haptic device
	Vector3 zero;
	Vector3 positionControlA; // x y z position
	Vector3 positionControlB; 
	Matrix3x3 rotationControlA; // rotation matrix
	Matrix3x3 rotationControlB; 
	Vector3 forceControlA; // x y z force
	Vector3 forceControlB;
	Vector3 torqueControlA; // x y z torque
	Vector3 torqueControlB;
	Vector3 translationDamping; // damping constants
	Vector3 rotationDamping;

	HapticDevice *haptDeviceA;
	HapticDevice *haptDeviceB;

	static const int sleepTime = 2000; //in microseconds

//private:

};
