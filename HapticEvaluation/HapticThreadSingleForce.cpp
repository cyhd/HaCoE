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
#include "HapticThreadSingleForce.h"

HapticThreadSingleForce::HapticThreadSingleForce( void ){}

HapticThreadSingleForce::~HapticThreadSingleForce( void ){}

//all constants copy Entact example mastermaster_main.cpp
const double HapticThreadSingleForce::SF_GENERAL_DAMPING_TRANSLATION = 0.0008; //damping constants
const double HapticThreadSingleForce::SF_GENERAL_DAMPING_ROTATION = 1.0;
const double HapticThreadSingleForce::SF_K_FORCE = 0.06; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double HapticThreadSingleForce::SF_K_TORQUE = 60;

void HapticThreadSingleForce::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();

	zero.x = 0;
	zero.y = 0;
	zero.z = 0;

	//This block sets up for Single Link Control (master-slave) and Dual Link Control (master-master) in Force control... Move to a different file later on
	translationDamping.x = SF_GENERAL_DAMPING_TRANSLATION; //damping constants for the Entact
	translationDamping.y = SF_GENERAL_DAMPING_TRANSLATION;
	translationDamping.z = SF_GENERAL_DAMPING_TRANSLATION;
	rotationDamping.x = SF_GENERAL_DAMPING_ROTATION;
	rotationDamping.y = SF_GENERAL_DAMPING_ROTATION;
	rotationDamping.z = SF_GENERAL_DAMPING_ROTATION;
	haptDeviceA->setMode( FORCECONTROL_MODE ); //A in Force Control.  This is the master.
	haptDeviceB->setMode( FORCECONTROL_MODE ); //B in Position Control.  This is the slave.	
	haptDeviceA->writeDamping( translationDamping , rotationDamping ); //sets damping to the two devices.
	haptDeviceB->writeDamping( translationDamping , rotationDamping );

	while( supervisor->getThreadStarted() )
	{		
		//This block is for single link control (Master-Slave) in force mode
		//Master position info only gets updated every time the foce is written
		haptDeviceA->readData();
		haptDeviceB->readData();

		supervisor->getMutex()->lock();
		transA = haptDeviceA->getTranslation();
		transB = haptDeviceB->getTranslation();
		rotA = haptDeviceA->getRotationMatrix();
		rotB = haptDeviceB->getRotationMatrix();
		supervisor->getMutex()->unlock();		
		
		forceControlB.x = -SF_K_FORCE*( transB.x - transA.x ); //Slave follows the master
		forceControlB.y = -SF_K_FORCE*( transB.y - transA.y );
		forceControlB.z = -SF_K_FORCE*( transB.z - transA.z );
		t2 = SF_K_TORQUE * ( rotB.mat23 - rotA.mat23 ); //copied from Entact example code
		t5 = SF_K_TORQUE * ( rotB.mat33 - rotA.mat33 );
		t10 = SF_K_TORQUE * ( rotB.mat13 - rotA.mat13 );
		torqueControlB.x = ( t2 * rotB.mat33 - t5 * rotB.mat23 );
		torqueControlB.y = ( t5 * rotB.mat13 - t10 * rotB.mat33 );
		torqueControlB.z = ( t10 * rotB.mat23 - t2 * rotB.mat13 );
		
		haptDeviceA->writeForce( zero , zero );
		haptDeviceB->writeForce( forceControlB , torqueControlB );
		
		usleep( sleepTime );
	}
}