#include "haptlinksupervisor.h"
#include "OmniDevice.h"
#include "HapticThreadForceToNetwork.h"

HapticThreadForceToNetwork :: HapticThreadForceToNetwork( void ){}

HapticThreadForceToNetwork :: ~HapticThreadForceToNetwork( void ){}

void HapticThreadForceToNetwork::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();

	zero.x = 0;
	zero.y = 0;
	zero.z = 0;


	while( supervisor->getThreadStarted() )
	{
		//This block is for dual link control (master-master) in force mode
		haptDeviceA->readData();
		haptDeviceB->readData();
		
		supervisor->getMutex()->lock(); // start mutex
		transA = haptDeviceA->getTranslation();
		transB = haptDeviceB->getTranslation();
		supervisor->getMutex()->unlock();//end mutex


		haptDeviceA->writeForce( forceControlA , torqueControlA );
		haptDeviceB->writeForce( forceControlB , torqueControlB );

		usleep( sleepTime );
	}
}