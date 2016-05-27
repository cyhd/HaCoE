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
#include "HapticThreadSingleHaptic.h"
#include "math.h"
#include "millisectime.h"


const double HapticThreadSingleHaptic::HR_GENERAL_DAMPING_TRANSLATION = 0.0008; //damping constants
const double HapticThreadSingleHaptic::HR_GENERAL_DAMPING_ROTATION = 1.0;
const double HapticThreadSingleHaptic::X_HOME = 218.3; //x zero position is 218.3mm
const double HapticThreadSingleHaptic::Y_HOME = 0;
const double HapticThreadSingleHaptic::Z_HOME = 0;

HapticThreadSingleHaptic::HapticThreadSingleHaptic( void )
{
	seqSwitch = false;
}

HapticThreadSingleHaptic::~HapticThreadSingleHaptic( void ){}


/*
double linearInterpolate(double tforce, double pforce, double progressive,double max) {
	double mu = progressive/max;
	double force = (pforce*(1-mu)+tforce*mu);
	return	force;
}

double cubicInterpolate(double tforce, float pforce, double progressive,double max) {
 
	double mu = progressive / max;
	double y0=pforce;
	double y1=pforce;
	double y2=tforce;
	double y3=2*tforce;
	double mu2= mu*mu;
	double a0 = y3 - y2 -y0 + y1;
	double a1 = y0 -y1 -a0;
	double a2 = y2 - y0;
	double a3 = y1;

	return(a0*mu*mu2+a1*mu2+a2*mu+a3);
}

double interpolate(double tforce, double pforce, double progressive, double max) {
 return linearInterpolate(tforce, pforce, progressive, max);
 //return cubicInterpolate(force, progressive, max);
}
*/

void HapticThreadSingleHaptic::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();
	Vector3 positionControl,positionControlzero;
	double depth = supervisor->getDistance();
	// force =  supervisor->getHaptRepF();
	double alpha =  supervisor->getDeltaDepth();
	
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
		
	//  experimentType == SINGLEHAPTIC) {
	outputForceDirection = supervisor->getHaptRepF();
	outputTorque = zero;
	outputForceMagnitude = sqrt( pow( outputForceDirection.x , 2 ) + pow( outputForceDirection.y , 2 ) + pow( outputForceDirection.z , 2 ) );//force magnitude calculation, to be used in unit vector calculation
	outputForceUnitVector.x = outputForceDirection.x/outputForceMagnitude;//Force unit vector calculation
	outputForceUnitVector.y = outputForceDirection.y/outputForceMagnitude;
	outputForceUnitVector.z = outputForceDirection.z/outputForceMagnitude;
	
	//outputDevice->writeForce( outputForce , outputTorque );
	outputDevice->readData();
	positionControlzero = outputDevice->getTranslation();
	
	timeStart = MilliSecTime::getInstance()->GetMilliCount();

	while ( supervisor->getThreadStarted() )
	{
		outputForce = supervisor->getHaptRepF();
		outputTorque = supervisor->getHaptRepT();
				
		outputForceDirection = outputForce;
		outputForceMagnitude = sqrt( pow( outputForceDirection.x , 2 ) + pow( outputForceDirection.y , 2 ) + pow( outputForceDirection.z , 2 ) );//force magnitude calculation, to be used in unit vector calculation
		outputForceUnitVector.x = outputForceDirection.x/outputForceMagnitude;//Force unit vector calculation
		outputForceUnitVector.y = outputForceDirection.y/outputForceMagnitude;
		outputForceUnitVector.z = outputForceDirection.z/outputForceMagnitude;
			
		outputDevice->readData();
		
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