/**********************************************************************************************************
HapticThreadRepSeq class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class is used to control perform a passive haptic replication experiment in sequential mode.

HaptDeviceA will produce a force which the subject must resist.
HaptDeviceA will stop producing the force.
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
#include "HapticThreadHaptReplication.h"
#include <QKeyEvent>

class HapticThreadRepSeq : public HapticThread , public HapticThreadHaptReplication
{
public:
	HapticThreadRepSeq();
	~HapticThreadRepSeq();
	void setK_FORCE( double k );
	void setK_TORQUE( double k );
private:
	double RS_K_FORCE;
	double RS_K_TORQUE;
};