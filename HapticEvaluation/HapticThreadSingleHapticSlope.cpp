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
#include "HapticThreadSingleHapticSlope.h"
#include "math.h"
#include "millisectime.h"


const double HapticThreadSingleHapticSlope::HR_GENERAL_DAMPING_TRANSLATION = 0.0008; //damping constants
const double HapticThreadSingleHapticSlope::HR_GENERAL_DAMPING_ROTATION = 1.0;
const double HapticThreadSingleHapticSlope::X_HOME = 218.3; //x zero position is 218.3mm
const double HapticThreadSingleHapticSlope::Y_HOME = 0;
const double HapticThreadSingleHapticSlope::Z_HOME = 0;

HapticThreadSingleHapticSlope::HapticThreadSingleHapticSlope( void )
{
	seqSwitch = false;
}

HapticThreadSingleHapticSlope::~HapticThreadSingleHapticSlope( void ){}



double HapticThreadSingleHapticSlope::interpolate(double z, double z0, double slope) {
	if (z <= z0) {
		return slope * (z0-z);
	}
	else {
		return 0;
	}
}



void HapticThreadSingleHapticSlope::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();
	Vector3 positionControl,positionControlzero;

	// readings from the user inputs
	double depth = supervisor->getDistance();
	double slope =  supervisor->getDeltaDepth(); // for linear force application
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

	// experimentType == DEPTHLINEAR) 
	outputForce.x = 0;
	outputForce.y = 0;
	outputForce.z = fMIN;
	
	outputTorque = zero;
	outputForceMagnitude = sqrt( pow( outputForceDirection.x , 2 ) + pow( outputForceDirection.y , 2 ) + pow( outputForceDirection.z , 2 ) );//force magnitude calculation, to be used in unit vector calculation
	outputForceUnitVector.x = outputForceDirection.x/outputForceMagnitude;//Force unit vector calculation
	outputForceUnitVector.y = outputForceDirection.y/outputForceMagnitude;
	outputForceUnitVector.z = outputForceDirection.z/outputForceMagnitude;
	
	outputDevice->writeForce( outputForce , outputTorque );
	usleep( 10*sleepTime );
	outputDevice->readData();

	// HOME to the start position
	supervisor->getMutex()->lock();
	positionControlzero = outputDevice->getTranslation();
	supervisor->getMutex()->unlock();
	
	
	timeStart = MilliSecTime::getInstance()->GetMilliCount();

	while ( supervisor->getThreadStarted() )
	{
		outputDevice->readData();
		
		supervisor->getMutex()->lock();
		positionControl = outputDevice->getTranslation();
		supervisor->getMutex()->unlock();
	
		outputForce.x = 0;
		outputForce.y = 0;
		outputForce.z = fMIN + interpolate(positionControl.z, positionControlzero.z, slope);
		
		outputForceDirection = outputForce;
		outputForceMagnitude = sqrt( pow( outputForceDirection.x , 2 ) + pow( outputForceDirection.y , 2 ) + pow( outputForceDirection.z , 2 ) );//force magnitude calculation, to be used in unit vector calculation
		outputForceUnitVector.x = outputForceDirection.x/outputForceMagnitude;//Force unit vector calculation
		outputForceUnitVector.y = outputForceDirection.y/outputForceMagnitude;
		outputForceUnitVector.z = outputForceDirection.z/outputForceMagnitude;
		
		supervisor->setAddedForceB(outputForce.z);

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