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

#ifndef DEVICE_H
#define DEVICE_H

#include "datamodel.h"

enum deviceType { ATI_DEVICE , HAPTIC_DEVICE, OMNI_DEVICE };

class Device
{
public:
	static const short SUCCESS = 1;
	static const short FAILED = 0;

	Vector3 getTranslation() const { return translation; }
    Vector3 getVelocity() const { return velocity; }
	Vector3 getRotation() const { return rotation; }
	Matrix3x3 getRotationMatrix() const { return rotationMatrix; }

	virtual short readData() = 0;
    virtual void closeConnection() = 0;
	virtual bool getConnectSuccess() const = 0;

	bool is(deviceType ptype) { return (ptype == type); }

protected:

	Device( deviceType device )
	{
		type = device;
	}
	
	void setTranslation(float x, float y, float z)   // a device has properties in xyz translation
	{ 
		translation.x = x;  
		translation.y = y;  
		translation.z = z; 
	} 

	void setVelocity(float x, float y, float z)   // a device has properties in xyz translation
	{ 
		velocity.x = x;  
		velocity.y = y;  
		velocity.z = z; 
	}

	void setRotation(float x, float y, float z)  // a device also has properties in xyz rotation
	{ 
		rotation.x = x;  
		rotation.y = y;  
		rotation.z = z; 
	} 

	void setRotationMatrix( double mat11 , double mat12 , double mat13 , 
							double mat21 , double mat22 , double mat23 , 
							double mat31 , double mat32 , double mat33 ) //certain devices have their rotation expressed as a matrix
	{
		rotationMatrix.mat11 = mat11;
		rotationMatrix.mat12 = mat12;
		rotationMatrix.mat13 = mat13;
		rotationMatrix.mat21 = mat21;
		rotationMatrix.mat22 = mat22;
		rotationMatrix.mat23 = mat23;
		rotationMatrix.mat31 = mat31;
		rotationMatrix.mat32 = mat32;
		rotationMatrix.mat33 = mat33;
	}
	
private:
	
	//empty Constructor
	Device(){} //we never construct a device alone.  We will instantiate a type of device.

    Vector3 translation;
	Vector3 velocity;
	Vector3 rotation;
	Matrix3x3 rotationMatrix;
	//Vector3 rotationRow1;
	//Vector3 rotationRow2;
	//Vector3 rotationRow3;
	deviceType type;
    
};

#endif // DEVICE_H
