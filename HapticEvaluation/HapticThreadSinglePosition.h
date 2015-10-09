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
HapticThreadSinglePosition class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class is used to control the haptic devices in Single Link (master-slave) control in Position Mode.

After the user chooses which haptic devices to activate and presses start on the GUI, and if the user
chooses to run Single Link Position Control, this thread will run.

Both Entact devices must be activated for this thread to run properly.

This thread does not instantiate the haptic devices.  It only manages the control aspects (gathering sensor 
information, calculation of force vectors, and commanding motion of the haptic devices).  The main 
functionality of the thread is implemented in the run function.

The thread will poll the GUI through haptlinksupervisor for the stop command while running.  If a stop command
is detected, then the thread stops.

haptDeviceA is in force mode.
haptDeviceB is in position mode.
*************************************************************************************************************/
#pragma once
#include <qthread.h>
#include "HapticThread.h"

class HapticThreadSinglePosition : public HapticThread
{
public:
	HapticThreadSinglePosition( void );
	~HapticThreadSinglePosition( void );
	void run();
};