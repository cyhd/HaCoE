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

#include "EntactDevice.h"
//#include "haptlinksupervisor.h"
#include "math.h"

eapi_device_handle EntactDevice::handle[2] = {};

EntactDevice::EntactDevice( int index , char *ip_address )
{
	entactIndex = index;
	if ( connectDeviceEAPI( handle , index , ip_address ) == 1 )//connect to the Entact Device.  Method provided by Entact
	{//only do this stuff if connection is successful
		entactIP = ip_address;
		connectSuccess = true;
		
		setModeEAPI( getEntactHandle() , DISABLED_MODE ); //connected, but keep things off.  Method provided by Entact
	}
	else 
		connectSuccess = false;
}

EntactDevice::~EntactDevice( void )  //make sure to close Entact when disconnecting to free memory
{
	EntactDevice::closeConnection();
}

void EntactDevice::closeConnection( void )
{
	setMode( DISABLED_MODE );
	handle[ getIndex() ] = 0;
}

//public setters
int EntactDevice::setMode( HapticMode setmode ) //sets directly to the Entact
{
	return setModeEAPI( handle[ getIndex() ] , setmode );  // Method provided by Entact
}

void EntactDevice::writeForce( Vector3 force , Vector3 torque )
{
	motionIn[0] = force.x;  // forces
	motionIn[1] = force.y;
	motionIn[2] = force.z;

	motionIn[3] = torque.x;
	motionIn[4] = torque.y;
	motionIn[5] = torque.z;

	writeForceEAPI( getEntactHandle() , motionIn , 6 ); //send the force to the entact
}

void EntactDevice::writePosition( Vector3 position , Matrix3x3 rotation )
{
	positionIn[0] = position.x;
	positionIn[1] = position.y;
	positionIn[2] = position.z;
	positionIn[3] = rotation.mat11;
	positionIn[4] = rotation.mat12;
	positionIn[5] = rotation.mat13;
	positionIn[6] = rotation.mat21;
	positionIn[7] = rotation.mat22;
	positionIn[8] = rotation.mat23;
	positionIn[9] = rotation.mat31;
	positionIn[10] = rotation.mat32;
	positionIn[11] = rotation.mat33;

	writePositionEAPI( getEntactHandle() , positionIn , 12 );
}

void EntactDevice::writeDamping( Vector3 translation , Vector3 rotation )
{
	
	damping[0] = translation.x;
	damping[1] = translation.y;
	damping[2] = translation.z;
	damping[3] = rotation.x;
	damping[4] = rotation.y;
	damping[5] = rotation.z;

	setDampingEAPI( getEntactHandle() , damping , 6 );
}

short EntactDevice::readData( void )
{
	if ( getMode() != EAPI_FORCECONTROL_MODE )
	{
		this->setTranslation( 0 , 0 , 0 );
		this->setRotation( 0 , 0 , 0 );
	}
	else 
	{
		readTaskPositionEAPI( getEntactHandle() , position , 12 );
		this->setTranslation( position[0] , position[1] , position[2] ); //these positions are given in millimetres.  Send back the translation info first
		this->setRotationMatrix( position[3] , position[4] , position[5] , 
								 position[6] , position[7] , position[8] ,
								 position[9] , position[10] , position[11] );  //set dummy values until I figure out how to extract angles for each axis in a rotation matrix
		this->setRotation( position[5] , position[8] , position[11] ); //rotation is shown as vector of the stylus of the device
		/* //Uncomment this next block in order to read velocities instead of positions.
		readTaskVelocity( haptHandle , velocity , 6 );
		this->setTranslation( velocity[0] , velocity[1] , velocity[2] );
		this->setRotation( velocity[3] , velocity [4] , velocity[5] );
		*/
	}
	return SUCCESS;
}

//private setters

//calibration
int EntactDevice::calibrate( void )
{
	return homeDeviceEAPI( getEntactHandle() ); //Method provided by Entact
}