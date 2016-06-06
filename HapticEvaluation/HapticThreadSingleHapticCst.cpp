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

#include <iostream>
#include "Device.h"
#include "haptlinksupervisor.h"
#include "hapticevaluationgui.h"
#include "EntactDevice.h"
#include "HapticThreadSingleHapticCst.h"
#include "math.h"
#include "millisectime.h"
#include "datalogger.h"

const double HapticThreadSingleHapticCst::HR_GENERAL_DAMPING_TRANSLATION = 0.0008; //damping constants
const double HapticThreadSingleHapticCst::HR_GENERAL_DAMPING_ROTATION = 1.0;
const double HapticThreadSingleHapticCst::X_HOME = 218.3; //x zero position is 218.3mm
const double HapticThreadSingleHapticCst::Y_HOME = 0;
const double HapticThreadSingleHapticCst::Z_HOME = 0;

HapticThreadSingleHapticCst::HapticThreadSingleHapticCst( void )
{
	seqSwitch = false;
	DataLogger::getInstance()->setLog(false);
}

HapticThreadSingleHapticCst::~HapticThreadSingleHapticCst( void ){}

double HapticThreadSingleHapticCst::interpolate(double z, double z0, double depth, double delta) {
	double zz = z0 - z;
	if (zz <= depth-delta) {
		return 0.0;
	} else if (zz < depth+delta) {
		return ((zz-depth+delta)/(2.0*delta));
	}
	else {
		return 1.0;
	}
}

void HapticThreadSingleHapticCst::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();
	Vector3 positionControl,positionControlzero;
	
	// readings from the user input
	double depth = supervisor->getDistance();
	Vector3 force =  supervisor->getHaptRepF();
	double alpha =  supervisor->getDeltaDepth();
	double fMIN = supervisor->getForceMin(); // minimal force always applied
	
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	
	outputDevice = haptDeviceB;
		
	translationDamping.x = HR_GENERAL_DAMPING_TRANSLATION; //damping constants for the Entact
	translationDamping.y = HR_GENERAL_DAMPING_TRANSLATION;
	translationDamping.z = HR_GENERAL_DAMPING_TRANSLATION;
	rotationDamping.x = HR_GENERAL_DAMPING_ROTATION;
	rotationDamping.y = HR_GENERAL_DAMPING_ROTATION;
	rotationDamping.z = HR_GENERAL_DAMPING_ROTATION;

	outputDevice->setMode( FORCECONTROL_MODE ); //A in Force Control. 
	outputDevice->writeDamping( translationDamping , rotationDamping ); //sets damping to the device

	//(experimentType == DEPTHCONST) {
	outputForce.x = 0;
	outputForce.y = 0;
	outputForce.z = fMIN;

	outputTorque = zero;
	/*
	outputForceMagnitude = sqrt( pow( outputForceDirection.x , 2 ) + pow( outputForceDirection.y , 2 ) + pow( outputForceDirection.z , 2 ) );//force magnitude calculation, to be used in unit vector calculation
	outputForceUnitVector.x = outputForceDirection.x/outputForceMagnitude;//Force unit vector calculation
	outputForceUnitVector.y = outputForceDirection.y/outputForceMagnitude;
	outputForceUnitVector.z = outputForceDirection.z/outputForceMagnitude;
	*/
	outputDevice->writeForce( outputForce , outputTorque );
	usleep( 10*sleepTime );
	outputDevice->readData();
		
	// HOME to the start position
	//positionControlzero = supervisor->getPositionControlzeroB();
	supervisor->getMutexB()->lock();
	positionControlzero = haptDeviceB->getTranslation();
	supervisor->getMutexB()->unlock();

		
	timeStart = MilliSecTime::getInstance()->GetMilliCount();

	// GRIFFITH z0 hard coded
	//positionControlzero.z = 37;

	while ( supervisor->getThreadStarted() )
	{
		outputDevice->readData();
		
		supervisor->getMutexA()->lock();
		positionControl = outputDevice->getTranslation();
		supervisor->getMutexA()->unlock();
	
		positionControlzero = supervisor->getPositionControlzeroB();

		outputForce.x = 0; // TODO Vector3
		outputForce.y = 0; // TODO Vector3
		outputForce.z = fMIN + force.z * interpolate(positionControl.z, positionControlzero.z, depth, fabs(alpha/2.0));
		
		/*
		outputForceDirection = outputForce;
		outputForceMagnitude = sqrt( pow( outputForceDirection.x , 2 ) + pow( outputForceDirection.y , 2 ) + pow( outputForceDirection.z , 2 ) );//force magnitude calculation, to be used in unit vector calculation
		outputForceUnitVector.x = outputForceDirection.x/outputForceMagnitude;//Force unit vector calculation
		outputForceUnitVector.y = outputForceDirection.y/outputForceMagnitude;
		outputForceUnitVector.z = outputForceDirection.z/outputForceMagnitude;
		*/

		// TODO Vector3
		supervisor->setAddedForceB(outputForce.z);

		//outputDevice->readData();
		
		outputDevice->writeForce( outputForce , outputTorque );
		
		currentTime = MilliSecTime::getInstance()->GetMilliCount();
		if ( ( currentTime - timeStart ) > SWITCH_TIME && !seqSwitch )
		{
			supervisor->GUINotify( HR_SIM_START); //HR_SEQ_SWITCH );
			seqSwitch = true;
		}
		
		usleep( sleepTime );
	}
}