#include "haptlinksupervisor.h"
#include "HapticThreadForceToNetwork.h"
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
	
	threadWrite = new WriteNetworkThread(ip, port, sleepTime/2, timeDelay);
	threadWrite->start( QThread::HighestPriority );
		
}

void HapticThreadForceToNetwork::initUDPRead(unsigned short port)
{
	
	threadRead = new ReadNetworkThread(port, sleepTime/4);
	threadRead->start( QThread::HighestPriority );
	
}


void HapticThreadForceToNetwork::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	
	haptDeviceA = supervisor->getHaptDeviceA(); // Real haptic device

	RemoteControlLaw *command = supervisor->getCommand(); 
	command->setSampleTime(sleepTime/2); //set the sample time in us

	while( supervisor->getThreadStarted() )
	{
		//This block is for dual link control (master-master) in position mode
		haptDeviceA->readData();
		
		// the position of both haptic devices are taken
		supervisor->getMutexA()->lock();
		position = haptDeviceA->getTranslation();
		velocity = haptDeviceA->getVelocity();
		supervisor->getMutexA()->unlock();

		supervisor->getMutexCommand()->lock();
		command->setData(position, LOCAL_POSITION);
		command->setData(velocity, LOCAL_VELOCITY);
		command->compute();

		supervisor->getMutexA()->lock();
		haptDeviceA->writeForce( command->getData(LOCAL_APPLIED_FORCE) , torqueControlA );
		supervisor->getMutexA()->unlock();
		supervisor->getMutexCommand()->unlock();
		
		usleep( sleepTime/2 );
	}
}