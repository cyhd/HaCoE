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
HapticThreadHaptReplication class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class is used to control perform a passive haptic replication experiment.

HaptDeviceA will produce a force which the subject must resist.
HaptDeviceB will hold its position and the subject must replicate the resisting force on HaptDeviceB.

After the user chooses which haptic devices to activate and presses start on the GUI, and if the user
chooses to run the Haptic Replication experiment, this thread will run.

Both Entact devices must be activated for this thread to run properly.

This thread does not instantiate the haptic devices.  It only manages the control aspects (gathering sensor 
information, calculation of force vectors, and commanding motion of the haptic devices).  The main 
functionality of the thread is implemented in the run function.

The thread will poll the GUI through haptlinksupervisor for the stop command while running.  If a stop command
is detected, then the thread stops.
*************************************************************************************************************/

#pragma once
#include <qthread.h>
#include "HapticThread.h"
#include <QKeyEvent>

class HapticThreadHaptReplication : public HapticThread
{
public:
	HapticThreadHaptReplication( void );
	~HapticThreadHaptReplication( void );
	void run();
protected:
	Vector3 outputForce; 
	Vector3 outputForceDirection; //a copy of the output force vector, but it is used only for its direction
	Vector3 outputTorque;
	Vector3 replicateForce; //the amount of force to output to resist movement on replicating device
	Vector3 replicateTorque;
	HapticDevice *outputDevice;
	HapticDevice *replicateDevice;

	double outputForceMagnitude; //used to computer unit vector
	Vector3 outputForceUnitVector; //unit vector in direction of output force
	Vector3 deviation; //absolute position w.r.t. home position
	double dotProduct; //used to compute the projection of the deviation from home onto vector parallel to the output force

	Vector3 trans; //position of the replicate device (translational)
	Matrix3x3 rot; //position of the replicate device (rotational)

	double t2;
	double t5;
	double t10;

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