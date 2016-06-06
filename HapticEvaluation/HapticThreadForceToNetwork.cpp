#include "haptlinksupervisor.h"
#include "OmniDevice.h"
#include "HapticThreadForceToNetwork.h"
#include "RemoteControlLawSimple.h"
#include "WriteNetworkThread.h"
#include "ReadNetworkThread.h"

HapticThreadForceToNetwork :: HapticThreadForceToNetwork( void ) {
HapticThreadForceToNetworkStarted = false;
}

HapticThreadForceToNetwork :: ~HapticThreadForceToNetwork( void ){}


void HapticThreadForceToNetwork::initUDPReadWrite(unsigned short portREAD, std::string ip, std::string portWRITE)
{
	initUDPWrite( ip, portWRITE);
	initUDPRead( portREAD);		
	HapticThreadForceToNetworkStarted = true;
}

void HapticThreadForceToNetwork::initUDPWrite(std::string ip, std::string port)
{
	
	threadWrite = new WriteNetworkThread(ip, port, sleepTime);
	threadWrite->start( QThread::HighestPriority );
		
}

void HapticThreadForceToNetwork::initUDPRead(unsigned short port)
{
	
	threadRead = new ReadNetworkThread(port, sleepTime);
	threadRead->start( QThread::HighestPriority );
	
}

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
		
		
		supervisor->getMutexA()->lock();
		transA = haptDeviceA->getTranslation();
		supervisor->getMutexA()->unlock();
		
		supervisor->getMutexB()->lock();
		transB = haptDeviceB->getTranslation();
		supervisor->getMutexB()->unlock();
		

		//supervisor->getMutex()->unlock();//end mutex

		forceControlA = command->getForce(transA, transB);

		//supervisor->getMutexA()->lock();
		haptDeviceA->writeForce( forceControlA , torqueControlA );
		//supervisor->getMutexA()->unlock();

		usleep( sleepTime );
	}
}