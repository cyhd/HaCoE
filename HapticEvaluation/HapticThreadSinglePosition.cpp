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
#include "HapticThreadSinglePosition.h"

HapticThreadSinglePosition::HapticThreadSinglePosition( void ){}

HapticThreadSinglePosition::~HapticThreadSinglePosition( void ){}

void HapticThreadSinglePosition::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();

	zero.x = 0;
	zero.y = 0;
	zero.z = 0;

	//This block sets up for Single Link Control (master-slave) in Position control... Move to a different file later on
	supervisor->getHaptDeviceA()->setMode( FORCECONTROL_MODE ); //A in Force Control.  This is the master.
	haptDeviceB->setMode( POSITIONCONTROL_MODE ); //B in Position Control.  This is the slave.

	while( supervisor->getThreadStarted() )
	{
		//This block is for single link control (Master-Slave) in position mode
		haptDeviceA->writeForce( zero , zero ); //Master position info only gets updated every time the force is written
		
		haptDeviceA->readData();  //get Master's position info
		
		supervisor->getMutexA()->lock();
		positionControlB = haptDeviceA->getTranslation();
		rotationControlB = haptDeviceA->getRotationMatrix();
		supervisor->getMutexA()->unlock();

		haptDeviceB->writePosition( positionControlB , rotationControlB );

		usleep( sleepTime );
	}
}