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

#include "Device.h"
#include "haptlinksupervisor.h"
#include "EntactDevice.h"
#include "HapticThreadHaptReplication.h"
#include "math.h"
#include "millisectime.h"


const double HapticThreadHaptReplication::HR_GENERAL_DAMPING_TRANSLATION = 0.0008; //damping constants
const double HapticThreadHaptReplication::HR_GENERAL_DAMPING_ROTATION = 1.0;
const double HapticThreadHaptReplication::X_HOME = 218.3; //x zero position is 218.3mm
const double HapticThreadHaptReplication::Y_HOME = 0;
const double HapticThreadHaptReplication::Z_HOME = 0;

HapticThreadHaptReplication::HapticThreadHaptReplication( void )
{
	seqSwitch = false;
}

HapticThreadHaptReplication::~HapticThreadHaptReplication( void ){}

void HapticThreadHaptReplication::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();

	zero.x = 0;
	zero.y = 0;
	zero.z = 0;
	
	if ( supervisor->getSideOut() == RIGHT )
	{
		outputDevice = haptDeviceB;
		replicateDevice = haptDeviceA;
	}
	else if ( supervisor->getSideOut() == LEFT )
	{
		outputDevice = haptDeviceA;
		replicateDevice = haptDeviceB;
	}

	outputDevice->setMode( FORCECONTROL_MODE );
	replicateDevice->setMode( FORCECONTROL_MODE );

	outputForceDirection = supervisor->getHaptRepF();
	outputForceMagnitude = sqrt( pow( outputForceDirection.x , 2 ) + pow( outputForceDirection.y , 2 ) + pow( outputForceDirection.z , 2 ) );//force magnitude calculation, to be used in unit vector calculation
	outputForceUnitVector.x = outputForceDirection.x/outputForceMagnitude;//Force unit vector calculation
	outputForceUnitVector.y = outputForceDirection.y/outputForceMagnitude;
	outputForceUnitVector.z = outputForceDirection.z/outputForceMagnitude;

	timeStart = MilliSecTime::getInstance()->GetMilliCount();

	while ( supervisor->getThreadStarted() )
	{
		outputForce = supervisor->getHaptRepF();
		outputTorque = supervisor->getHaptRepT();
		HR_K_FORCE = supervisor->getK_FORCE();
		HR_K_TORQUE = supervisor->getK_TORQUE();

		outputDevice->readData();
		replicateDevice->readData();

		supervisor->getMutexA()->lock();
		trans = replicateDevice->getTranslation();
		rot = replicateDevice->getRotationMatrix();
		supervisor->getMutexA()->unlock();

		replicateTorque = zero;
		//projection of the deviation from the plane normal to the outputForce
		deviation.x = trans.x - X_HOME;//b
		deviation.y = trans.y - Y_HOME;
		deviation.z = trans.z - Z_HOME;
		dotProduct = outputForceDirection.x * deviation.x + outputForceDirection.y * deviation.y + outputForceDirection.z * deviation.z; //magnitude of projection

		replicateForce.x = -HR_K_FORCE * ( dotProduct/outputForceMagnitude ) * outputForceUnitVector.x;
		replicateForce.y = -HR_K_FORCE * ( dotProduct/outputForceMagnitude ) * outputForceUnitVector.y;
		replicateForce.z = -HR_K_FORCE * ( dotProduct/outputForceMagnitude ) * outputForceUnitVector.z;
		
		/*
		replicateForce.x = -HR_K_FORCE*( trans.x - X_HOME ); //goes to home position
		replicateForce.y = -HR_K_FORCE*( trans.y - Y_HOME );
		replicateForce.z = -HR_K_FORCE*( trans.z - Z_HOME );
		t2 = HR_K_TORQUE * ( rot.mat23 - 0 ); // point towards user (in the x axis)
		t5 = HR_K_TORQUE * ( rot.mat33 - 0 );
		t10 = HR_K_TORQUE * ( rot.mat13 - 1 );
		replicateTorque.x = ( t2 * rot.mat33 - t5 * rot.mat23 );
		replicateTorque.y = ( t5 * rot.mat13 - t10 * rot.mat33 );
		replicateTorque.z = ( t10 * rot.mat23 - t2 * rot.mat13 );
		*/
		
		replicateDevice->writeForce( replicateForce , replicateTorque );
		outputDevice->writeForce( outputForce , outputTorque );
		
		currentTime = MilliSecTime::getInstance()->GetMilliCount();
		if ( ( currentTime - timeStart ) > SWITCH_TIME && !seqSwitch )
		{
			supervisor->GUINotify( HR_SEQ_SWITCH );
			seqSwitch = true;
		}
		
		usleep( sleepTime );
	}
}