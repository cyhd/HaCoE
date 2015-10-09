#include "Device.h"
#include "haptlinksupervisor.h"
#include "EntactDevice.h"
#include "HapticThreadDoublePosition.h"

HapticThreadDoublePosition::HapticThreadDoublePosition( void ){}

HapticThreadDoublePosition::~HapticThreadDoublePosition( void ){}

void HapticThreadDoublePosition::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();

	zero.x = 0;
	zero.y = 0;
	zero.z = 0;

	while( supervisor->getThreadStarted() )
	{
		//This block is for dual link control (master-master) in position mode
		haptDeviceA->setMode( EAPI_FORCECONTROL_MODE );
		haptDeviceB->setMode( EAPI_FORCECONTROL_MODE );
		haptDeviceA->writeForce( zero , zero );
		haptDeviceB->writeForce( zero , zero );
		haptDeviceA->readData();
		haptDeviceB->readData();

		positionControlA.x = ( haptDeviceA->getTranslation().x + haptDeviceB->getTranslation().x )/2;
		positionControlA.y = ( haptDeviceA->getTranslation().y + haptDeviceB->getTranslation().y )/2;
		positionControlA.z = ( haptDeviceA->getTranslation().z + haptDeviceB->getTranslation().z )/2;
		positionControlB.x = positionControlA.x;
		positionControlB.y = positionControlA.y;
		positionControlB.z = positionControlA.z;

		rotationControlA.mat11 = ( haptDeviceA->getRotationMatrix().mat11 + haptDeviceB->getRotationMatrix().mat11 )/2;
		rotationControlA.mat12 = ( haptDeviceA->getRotationMatrix().mat12 + haptDeviceB->getRotationMatrix().mat12 )/2;
		rotationControlA.mat13 = ( haptDeviceA->getRotationMatrix().mat13 + haptDeviceB->getRotationMatrix().mat13 )/2;
		rotationControlA.mat21 = ( haptDeviceA->getRotationMatrix().mat21 + haptDeviceB->getRotationMatrix().mat21 )/2;
		rotationControlA.mat22 = ( haptDeviceA->getRotationMatrix().mat22 + haptDeviceB->getRotationMatrix().mat22 )/2;
		rotationControlA.mat23 = ( haptDeviceA->getRotationMatrix().mat23 + haptDeviceB->getRotationMatrix().mat23 )/2;
		rotationControlA.mat31 = ( haptDeviceA->getRotationMatrix().mat31 + haptDeviceB->getRotationMatrix().mat31 )/2;
		rotationControlA.mat32 = ( haptDeviceA->getRotationMatrix().mat32 + haptDeviceB->getRotationMatrix().mat32 )/2;
		rotationControlA.mat33 = ( haptDeviceA->getRotationMatrix().mat33 + haptDeviceB->getRotationMatrix().mat33 )/2;
		
		rotationControlB.mat11 = rotationControlA.mat11;
		rotationControlB.mat12 = rotationControlA.mat12;
		rotationControlB.mat13 = rotationControlA.mat13;
		rotationControlB.mat21 = rotationControlA.mat21;
		rotationControlB.mat22 = rotationControlA.mat22;
		rotationControlB.mat23 = rotationControlA.mat23;
		rotationControlB.mat31 = rotationControlA.mat31;
		rotationControlB.mat32 = rotationControlA.mat32;
		rotationControlB.mat33 = rotationControlA.mat33;

		haptDeviceA->setMode( EAPI_POSITIONCONTROL_MODE );
		haptDeviceB->setMode( EAPI_POSITIONCONTROL_MODE );
		
		haptDeviceA->writePosition( positionControlA , rotationControlA );
		haptDeviceB->writePosition( positionControlB , rotationControlB );

		usleep(2000);
	}
}