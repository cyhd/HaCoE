#include "haptlinksupervisor.h"
#include "OmniDevice.h"
#include "HapticThreadForceToNetwork.h"
#include "RemoteControlLawSimple.h"


HapticThreadForceToNetwork :: HapticThreadForceToNetwork( void ){}

HapticThreadForceToNetwork :: ~HapticThreadForceToNetwork( void ){}

/*
const double HapticThreadForceToNetwork::F2N_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double HapticThreadForceToNetwork::F2N_SCALERFORCE = 1; //SCALERFORCE is used to modify the stiffness difference between the two different haptic devices
*/
void HapticThreadForceToNetwork::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	
	haptDeviceA = supervisor->getHaptDeviceA(); // Real haptic device
	haptDeviceB = supervisor->getHaptDeviceB(); // Remote haptic device

	RemoteControlLaw *command = new RemoteControlLawSimple(); 
	
	while( supervisor->getThreadStarted() )
	{
		//This block is for dual link control (master-master) in force mode
		haptDeviceA->readData();
		
		supervisor->getMutex()->lock(); // start mutex
		transA = haptDeviceA->getTranslation();
		transB = haptDeviceB->getTranslation();
		supervisor->getMutex()->unlock();//end mutex

		forceControlA = command->getForce(transA, transB);

		haptDeviceA->writeForce( forceControlA , torqueControlA );
		
		usleep( sleepTime );
	}
}