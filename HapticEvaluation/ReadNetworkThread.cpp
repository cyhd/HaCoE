#include "ReadNetworkThread.h"
#include "haptlinksupervisor.h"





ReadNetworkThread::ReadNetworkThread(void) { }

ReadNetworkThread::~ReadNetworkThread(void) { }

void ReadNetworkThread::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	
	//create a new socket for communication
	

	while(supervisor->getThreadStarted())
	{
		haptDeviceA->readData();

		supervisor->getMutex()->lock();
		translation = haptDeviceA->getTranslation();
		supervisor->getMutex()->unlock();
		
		//id of the message
		

		usleep( sleepTime );
	}

}