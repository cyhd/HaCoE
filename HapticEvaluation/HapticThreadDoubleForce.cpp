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
#include "HapticThreadDoubleForce.h"

HapticThreadDoubleForce::HapticThreadDoubleForce( void ){}

HapticThreadDoubleForce::~HapticThreadDoubleForce( void ){}

//all constants copy Entact example mastermaster_main.cpp
const double HapticThreadDoubleForce::DF_GENERAL_DAMPING_TRANSLATION = 0.0008; //damping constants
const double HapticThreadDoubleForce::DF_GENERAL_DAMPING_ROTATION = 1.0;
const double HapticThreadDoubleForce::DF_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double HapticThreadDoubleForce::DF_K_TORQUE = 60;
const double HapticThreadDoubleForce::DF_SCALERFORCE = 1; //SCALERFORCE is used to modify the stiffness difference between the two different haptic devices

void HapticThreadDoubleForce::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();

	zero.x = 0;
	zero.y = 0;
	zero.z = 0;

	translationDamping.x = DF_GENERAL_DAMPING_TRANSLATION; //damping constants for the Entact
	translationDamping.y = DF_GENERAL_DAMPING_TRANSLATION;
	translationDamping.z = DF_GENERAL_DAMPING_TRANSLATION;
	rotationDamping.x = DF_GENERAL_DAMPING_ROTATION;
	rotationDamping.y = DF_GENERAL_DAMPING_ROTATION;
	rotationDamping.z = DF_GENERAL_DAMPING_ROTATION;

	haptDeviceA->setMode( EAPI_FORCECONTROL_MODE ); //A in Force Control.  This is the master.
	haptDeviceB->setMode( EAPI_FORCECONTROL_MODE ); //B in Position Control.  This is the slave.	
	haptDeviceA->writeDamping( translationDamping , rotationDamping ); //sets damping to the two devices.
	haptDeviceB->writeDamping( translationDamping , rotationDamping );


	while( supervisor->getThreadStarted() )
	{
		//This block is for dual link control (master-master) in force mode
		haptDeviceA->readData();
		haptDeviceB->readData();
		
		supervisor->getMutex()->lock();
		transA = haptDeviceA->getTranslation();
		transB = haptDeviceB->getTranslation();
		rotA = haptDeviceA->getRotationMatrix();
		rotB = haptDeviceB->getRotationMatrix();
		supervisor->getMutex()->unlock();//end mutex

		forceControlA.x = -DF_K_FORCE*( transA.x - transB.x ); 
		forceControlA.y = -DF_K_FORCE*( transA.y - transB.y );
		forceControlA.z = -DF_K_FORCE*( transA.z - transB.z );
		t2 = DF_K_TORQUE * ( rotA.mat23 - rotB.mat23 ); //copied from Entact example code
		t5 = DF_K_TORQUE * ( rotA.mat33 - rotB.mat33 );
		t10 = DF_K_TORQUE * ( rotA.mat13 - rotB.mat13 );
		torqueControlA.x = ( t2 * rotA.mat33 - t5 * rotA.mat23 );
		torqueControlA.y = ( t5 * rotA.mat13 - t10 * rotA.mat33 );
		torqueControlA.z = ( t10 * rotA.mat23 - t2 * rotA.mat13 );
		torqueControlB.x = - ( t2 * rotB.mat33 - t5 * rotB.mat23 );
		torqueControlB.y = - ( t5 * rotB.mat13 - t10 * rotB.mat33 );
		torqueControlB.z = - ( t10 * rotB.mat23 - t2 * rotB.mat13 );		
		
		forceControlB.x = -forceControlA.x;
		forceControlB.y = -forceControlA.y;
		forceControlB.z = -forceControlA.z;

		forceControlA.x = forceControlA.x * DF_SCALERFORCE;
		forceControlA.y = forceControlA.y * DF_SCALERFORCE;
		forceControlA.z = forceControlA.z * DF_SCALERFORCE;
		torqueControlA.x = torqueControlA.x * DF_SCALERFORCE;
		torqueControlA.y = torqueControlA.y * DF_SCALERFORCE;
		torqueControlA.z = torqueControlA.z * DF_SCALERFORCE;

		haptDeviceA->writeForce( forceControlA , torqueControlA );
		haptDeviceB->writeForce( forceControlB , torqueControlB );

		usleep( sleepTime );
	}
}