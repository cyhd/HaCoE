#include "WriteNetworkThread.h"

#include <QUANTA/QUANTAnet_udp_c.hxx>

	
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
	QUANTAnet_udp_c socket;
	socket.init(20010);
	socket.setSendAddress("192.168.1.10", 20011);

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
		socket.send("T",1,QUANTAnet_udp_c::NON_BLOCKING); //send translation
		socket.send("3",1,QUANTAnet_udp_c::NON_BLOCKING); //3 packets : x,y,z

		sprintf(translationCom[0], "%f", translation.x);
		sprintf(translationCom[1], "%f", translation.y);
		sprintf(translationCom[2], "%f", translation.z);

		for (int i=0; i<3; i++)
		{
			socket.send(translationCom[i],8,QUANTAnet_udp_c::NON_BLOCKING);
		}
		
		usleep( sleepTime );
	}

}