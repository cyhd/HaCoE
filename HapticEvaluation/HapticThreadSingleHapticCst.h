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
HapticThreadSingleHaptic class

Created: May 2014
Author: Cedric Dumas

This class is used to control perform a single haptic force feedback experiment.

dominante hand device will be used: HaptDeviceA or HaptDeviceB 

The thread will poll the GUI through haptlinksupervisor for the stop command while running.  If a stop command
is detected, then the thread stops.
*************************************************************************************************************/

#pragma once
#include <qthread.h>
#include "HapticThread.h"
#include <QKeyEvent>

class HapticThreadSingleHapticCst : public HapticThread
{
public:
	HapticThreadSingleHapticCst( void );
	~HapticThreadSingleHapticCst( void );
	void run();
protected:
	double interpolate(double z, double z0, double depth, double delta);

	Vector3 outputForce; 
	Vector3 outputForceDirection; //a copy of the output force vector, but it is used only for its direction
	Vector3 outputTorque;
	HapticDevice *outputDevice;
	
	double outputForceMagnitude; //used to computer unit vector
	Vector3 outputForceUnitVector; //unit vector in direction of output force
	
	static const double HR_GENERAL_DAMPING_TRANSLATION;
	static const double HR_GENERAL_DAMPING_ROTATION;
	static const double X_HOME;
	static const double Y_HOME;
	static const double Z_HOME;
	static const int SWITCH_TIME = 5000;

	double HR_K_FORCE;
	double HR_K_TORQUE;

private:
	//int count;
	bool seqSwitch;
	int timeStart;
	int currentTime;

};