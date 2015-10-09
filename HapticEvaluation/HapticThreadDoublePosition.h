/**********************************************************************************************************
HapticThreadDoublePosition class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class is used to control the haptic devices in Dual Link (master-master) control in Force Mode.
After the user chooses which haptic devices to activate and presses start on the GUI, this thread will run.
Both Entact devices should be activated for this thread to run properly.
This thread does not instantiate the haptic devices.  It only manages the control aspects (gathering sensor 
information, calculation of force vectors, and commanding motion of the haptic devices).  The main 
functionality of the thread is implemented in the run function.

The thread will poll the GUI through haptlinksupervisor for the stop command while running.  If a stop command
is detected, then the thread stops.
*************************************************************************************************************/
#pragma once
#include <qthread.h>
#include "HapticThread.h"

class HapticThreadDoublePosition : public HapticThread
{
public:
	HapticThreadDoublePosition( void );
	~HapticThreadDoublePosition( void );
	void run();
};