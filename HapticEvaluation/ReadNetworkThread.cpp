#include "ReadNetworkThread.h"
#include "haptlinksupervisor.h"

#include <QUANTA/QUANTAnet_udp_c.hxx>



ReadNetworkThread::ReadNetworkThread(void) { }

ReadNetworkThread::~ReadNetworkThread(void) { }

void ReadNetworkThread::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	
	//create a new socket for communication
	QUANTAnet_udp_c socket;
	socket.init(20010);

	while(supervisor->getThreadStarted())
	{
		haptDeviceA->readData();

		supervisor->getMutex()->lock();
		translation = haptDeviceA->getTranslation();
		supervisor->getMutex()->unlock();
		
		//id of the message
		socket.receive(packetID,1,QUANTAnet_udp_c::NON_BLOCKING); //send translation
		socket.receive(nbBytes,1,QUANTAnet_udp_c::NON_BLOCKING); //3 packets : x,y,z

		if((packetID,"T"))
		{
			for(int i=0; i<(int)nbBytes; i++)
			{
				socket.receive(translationCom[i],8,QUANTAnet_udp_c::NON_BLOCKING);
			}
		}

		usleep( sleepTime );
	}

}