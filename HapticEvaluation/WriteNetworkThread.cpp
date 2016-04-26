#include "WriteNetworkThread.h"


	
//#include "datamodel.h"
#include "Device.h"
//#include "OmniDevice.h"
#include "haptlinksupervisor.h"
//#include <iostream>

WriteNetworkThread::WriteNetworkThread(void){}

WriteNetworkThread::~WriteNetworkThread(void){}

void WriteNetworkThread::run()
{
	//create a new socket for communication
	
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	
	zero.x = 0;
	zero.y = 0;
	zero.z = 0;


	while(supervisor->getThreadStarted())
	{
		haptDeviceA->readData();
		supervisor->getMutex()->lock();
		translation = haptDeviceA->getTranslation();
		supervisor->getMutex()->unlock();
		
		//id of the message
		
		sprintf(translationCom[0], "%f", translation.x);
		sprintf(translationCom[1], "%f", translation.y);
		sprintf(translationCom[2], "%f", translation.z);

	
		usleep( sleepTime );
	}

}