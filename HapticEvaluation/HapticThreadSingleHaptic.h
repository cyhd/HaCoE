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

class HapticThreadSingleHaptic : public HapticThread
{
public:
	HapticThreadSingleHaptic( void );
	~HapticThreadSingleHaptic( void );
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