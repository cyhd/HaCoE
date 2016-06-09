#include "haptlinksupervisor.h"
#include "OmniDevice.h"
#include "HapticThreadForceToNetwork.h"
#include "RemoteControlLawSimple.h"
#include "RemoteControlLawSimpleFiltered.h"
#include "WriteNetworkThread.h"
#include "ReadNetworkThread.h"

HapticThreadForceToNetwork :: HapticThreadForceToNetwork( void ) {
HapticThreadForceToNetworkStarted = false;
}

HapticThreadForceToNetwork :: ~HapticThreadForceToNetwork( void ){}


void HapticThreadForceToNetwork::initUDPReadWrite(unsigned short portREAD, std::string ip, std::string portWRITE, int timeDelay)
{
	initUDPWrite( ip, portWRITE, timeDelay);
	initUDPRead( portREAD);		
	HapticThreadForceToNetworkStarted = true;
}

void HapticThreadForceToNetwork::initUDPWrite(std::string ip, std::string port, int timeDelay)
{
	
	threadWrite = new WriteNetworkThread(ip, port, sleepTime/2*10, timeDelay);
	threadWrite->start( QThread::HighestPriority );
		
}

void HapticThreadForceToNetwork::initUDPRead(unsigned short port)
{
	
	threadRead = new ReadNetworkThread(port, sleepTime/2);
	threadRead->start( QThread::HighestPriority );
	
}


void HapticThreadForceToNetwork::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	
	haptDeviceA = supervisor->getHaptDeviceA(); // Real haptic device
	haptDeviceB = supervisor->getHaptDeviceB(); // Remote haptic device

	haptDeviceA->calibrate();

	RemoteControlLaw *command = new RemoteControlLawSimple(); 
	//RemoteControlLaw *command = new RemoteControlLawSimpleFiltered();

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

		supervisor->getMutexA()->lock();
		haptDeviceA->writeForce( forceControlA , torqueControlA );
		supervisor->getMutexA()->unlock();

		usleep( sleepTime/2 );
	}
}