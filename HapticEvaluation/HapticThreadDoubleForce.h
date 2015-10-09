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
HapticThreadDoubleForce class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class is used to control the haptic devices in Dual Link (master-master) control in Force Mode.
After the user chooses which haptic devices to activate and presses start on the GUI, and if the user
chooses to run Dual Link Force Control, this thread will run.

Both Entact devices must be activated for this thread to run properly.

This thread does not instantiate the haptic devices.  It only manages the control aspects (gathering sensor 
information, calculation of force vectors, and commanding motion of the haptic devices).  The main 
functionality of the thread is implemented in the run function.

The thread will poll the GUI through haptlinksupervisor for the stop command while running.  If a stop command
is detected, then the thread stops.

Both haptDevices are in force mode.
haptDeviceA is the master.
haptDeviceB is the slave.
*************************************************************************************************************/
#pragma once
#include <qthread.h>
#include "HapticThread.h"

class HapticThreadDoubleForce : public HapticThread
{
public:
	HapticThreadDoubleForce( void );
	~HapticThreadDoubleForce( void );
	void run();
private:
	Vector3 transA; //translational position of haptic device
	Vector3 transB;
	Matrix3x3 rotA; //rotational position of haptic device
	Matrix3x3 rotB;
	double t2; //used in calculating the cross product to find how much the angle is deviated.
	double t5;
	double t10;
	static const double DF_GENERAL_DAMPING_TRANSLATION;
	static const double DF_GENERAL_DAMPING_ROTATION;
	static const double DF_K_FORCE;
	static const double DF_K_TORQUE;
	static const double DF_SCALERFORCE;
};