#include "Device.h"
#include "haptlinksupervisor.h"
#include "HapticThread.h" 
#include "EntactDevice.h"


//void HapticThread::run()
//{
//	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
//	haptDeviceA = supervisor->getHaptDeviceA();
//	haptDeviceB = supervisor->getHaptDeviceB();
//
//	zero.x = 0;
//	zero.y = 0;
//	zero.z = 0;
//
//	//This block sets up for Single Link Control (master-slave) in Position control... Move to a different file later on
//	//supervisor->getHaptDeviceA()->setMode( EAPI_FORCECONTROL_MODE ); //A in Force Control.  This is the master.
//	//haptDeviceB->setMode( EAPI_POSITIONCONTROL_MODE ); //B in Position Control.  This is the slave.
//
//	////This block sets up for Single Link Control (master-slave) and Dual Link Control (master-master) in Force control... Move to a different file later on
//	//K_FORCE = 0.08; //change K_FORCE and K_TORQUE to get different stiffness/tightness characteristics.  Higher values will cause more instability
//	//K_TORQUE = 60;
//	//SCALERFORCE = 1;
//	//translationDamping.x = 0.0008; //damping constants for the Entact
//	//translationDamping.y = 0.0008;
//	//translationDamping.z = 0.0008;
//	//rotationDamping.x = 1;
//	//rotationDamping.y = 1;
//	//rotationDamping.z = 1;
//	//haptDeviceA->setMode( EAPI_FORCECONTROL_MODE ); //A in Force Control.  This is the master.
//	//haptDeviceB->setMode( EAPI_FORCECONTROL_MODE ); //B in Position Control.  This is the slave.	
//	//haptDeviceA->writeDamping( translationDamping , rotationDamping ); //sets damping to the two devices.
//	//haptDeviceB->writeDamping( translationDamping , rotationDamping );	
//
//	//This block sets up for Dual Link Control (master-master) in Position control
//
//
//	//loop waiting for user event start 
//	while ( supervisor->getThreadStarted() )
//	{	
//		//This block is for single link control (Master-Slave) in position mode
//		//haptDeviceA->writeForce( zero , zero ); //Master position info only gets updated every time the force is written
//		//haptDeviceA->readData();  //Write Master's position info
//		//positionControlB = haptDeviceA->getTranslation();
//		//rotationControlB = haptDeviceA->getRotationMatrix();
//		//haptDeviceB->writePosition( positionControlB , rotationControlB );
//
//		//This block is for single link control (Master-Slave) in force mode
//		//Master position info only gets updated every time the foce is written
//		//haptDeviceA->readData();
//		//haptDeviceB->readData();
//		//forceControlB.x = -K_FORCE*( haptDeviceB->getTranslation().x - haptDeviceA->getTranslation().x ); //Slave follows the master
//		//forceControlB.y = -K_FORCE*( haptDeviceB->getTranslation().y - haptDeviceA->getTranslation().y );
//		//forceControlB.z = -K_FORCE*( haptDeviceB->getTranslation().z - haptDeviceA->getTranslation().z );
//		//double t2 = K_TORQUE * ( haptDeviceB->getRotationMatrix().mat23 - haptDeviceA->getRotationMatrix().mat23 ); //copied from Entact example code
//		//double t5 = K_TORQUE * ( haptDeviceB->getRotationMatrix().mat33 - haptDeviceA->getRotationMatrix().mat33 );
//		//double t10 = K_TORQUE * ( haptDeviceB->getRotationMatrix().mat13 - haptDeviceA->getRotationMatrix().mat13 );
//		//torqueControlB.x = ( t2 * haptDeviceB->getRotationMatrix().mat33 - t5 * haptDeviceB->getRotationMatrix().mat23 );
//		//torqueControlB.y = ( t5 * haptDeviceB->getRotationMatrix().mat13 - t10 * haptDeviceB->getRotationMatrix().mat33 );
//		//torqueControlB.z = ( t10 * haptDeviceB->getRotationMatrix().mat23 - t2 * haptDeviceB->getRotationMatrix().mat13 );
//		//haptDeviceA->writeForce( zero , zero );
//		//haptDeviceB->writeForce( forceControlB , torqueControlB );
//
//		//This block is for dual link control (master-master) in force mode
//		//haptDeviceA->readData();
//		//haptDeviceB->readData();
//		//forceControlA.x = -K_FORCE*( haptDeviceA->getTranslation().x - haptDeviceB->getTranslation().x ); 
//		//forceControlA.y = -K_FORCE*( haptDeviceA->getTranslation().y - haptDeviceB->getTranslation().y );
//		//forceControlA.z = -K_FORCE*( haptDeviceA->getTranslation().z - haptDeviceB->getTranslation().z );
//		//forceControlB.x = -forceControlA.x;
//		//forceControlB.y = -forceControlA.y;
//		//forceControlB.z = -forceControlA.z;
//		//double t2 = K_TORQUE * ( haptDeviceA->getRotationMatrix().mat23 - haptDeviceB->getRotationMatrix().mat23 ); //copied from Entact example code
//		//double t5 = K_TORQUE * ( haptDeviceA->getRotationMatrix().mat33 - haptDeviceB->getRotationMatrix().mat33 );
//		//double t10 = K_TORQUE * ( haptDeviceA->getRotationMatrix().mat13 - haptDeviceB->getRotationMatrix().mat13 );
//		//torqueControlA.x = ( t2 * haptDeviceA->getRotationMatrix().mat33 - t5 * haptDeviceA->getRotationMatrix().mat23 );
//		//torqueControlA.y = ( t5 * haptDeviceA->getRotationMatrix().mat13 - t10 * haptDeviceA->getRotationMatrix().mat33 );
//		//torqueControlA.z = ( t10 * haptDeviceA->getRotationMatrix().mat23 - t2 * haptDeviceA->getRotationMatrix().mat13 );
//		//torqueControlB.x = - ( t2 * haptDeviceB->getRotationMatrix().mat33 - t5 * haptDeviceB->getRotationMatrix().mat23 );
//		//torqueControlB.y = - ( t5 * haptDeviceB->getRotationMatrix().mat13 - t10 * haptDeviceB->getRotationMatrix().mat33 );
//		//torqueControlB.z = - ( t10 * haptDeviceB->getRotationMatrix().mat23 - t2 * haptDeviceB->getRotationMatrix().mat13 );
//		//forceControlA.x = forceControlA.x * SCALERFORCE;
//		//forceControlA.y = forceControlA.y * SCALERFORCE;
//		//forceControlA.z = forceControlA.z * SCALERFORCE;
//		//torqueControlA.x = torqueControlA.x * SCALERFORCE;
//		//torqueControlA.y = torqueControlA.y * SCALERFORCE;
//		//torqueControlA.z = torqueControlA.z * SCALERFORCE;
//		//haptDeviceA->writeForce( forceControlA , torqueControlA );
//		//haptDeviceB->writeForce( forceControlB , torqueControlB );
//
//		//This block is for dual link control
//			
//		//usleep(2000); // sleep time in microseconds, any smaller and the devices will begin to stutter.
//	}
//}