#include "WriteNetworkThread.h"

#include "Device.h"
#include <boost/asio/ip/udp.hpp>
#include "haptlinksupervisor.h"

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

using boost::asio::ip::udp;

WriteNetworkThread::WriteNetworkThread(
		std::string host,
		std::string port,
		int sleep, 
		int timeDelay
	) : io_service(),
      socket_(io_service, udp::endpoint(udp::v4(), 0))
{
	io_service.run();

	resolver =  new udp::resolver(io_service);
	query = new udp::resolver::query(udp::v4(), host, port);
	iter =  new udp::resolver::iterator(resolver->resolve(*query));
	endpoint_ = *(*iter);

	sleepTime = sleep; //sleep = 2ms
	
	//init the delay
	delayValue = timeDelay*1000/sleepTime;
	if (delayValue == 0)
		delayValue = 1;

	//init the buffers
	buffCpt=0;
	for (int i = 0; i < delayValue; i++)
	{
		for(int k = 0; k < 10; k++)
		{
			dataBuff[i][k] = Vector3(0.0, 0.0, 0.0);
		}
	}
	dataSwitch = LOCAL_POSITION;
};

WriteNetworkThread::~WriteNetworkThread()
{
	socket_.close();
}

void WriteNetworkThread::send(std::string str) 
{
	std::vector<char> char_buff(str.size(), 0);
	for (size_t i = 0; i < str.size(); i++)
	{
		char_buff[i] = str[i];
	}
	try 
	{
		socket_.send_to(boost::asio::buffer(char_buff, char_buff.size()), endpoint_);
	}
	catch(boost::system::system_error &e )
	{
		//ec.message();
		//throw;
		std::cerr<<"Exception RESEAU: "<<e.code()<<std::endl;
		this->terminate();
	}
}


/*************************************************************************** 
Write the new data in a buffer place that will be read only when the buffCpt
changed all the values of the buffer -> delayValue times
***************************************************************************/
Vector3 WriteNetworkThread::delay(Vector3 data, DataType type, int cpt)
{
	dataBuff[buffCpt][type] = data;
	if(cpt == dataNumber-1)
	{
		buffCpt++;
		if (buffCpt == delayValue)
			buffCpt = 0;
	}
	return dataBuff[buffCpt][type];
}

void WriteNetworkThread::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	HapticDevice *haptDeviceA = supervisor->getHaptDeviceA();

	RemoteControlLaw *command = supervisor->getCommand(); 
	dataNumber = command->getDataNumber();
	
	while( supervisor->getThreadStarted() )
	{
		for (int i = 0; i < dataNumber; i++)
		{
			/****************************************************
			The data to be sent changed depending on the 
			control law
			****************************************************/
			dataSwitch = command->send();
		
			/*********************************************************
			The first packet sent defines the type and number of data
			*********************************************************/
			switch(dataSwitch)
				{
				case LOCAL_POSITION :
					send("T3"); 
					break;
				case LOCAL_FORCE :
					send("F3"); 
					break;
				case LOCAL_VELOCITY :
					send("V3"); 
					break;
				case DESIRED_LOCAL_POSITION :
					send("P3"); 
					break;
				case LOCAL_APPLIED_FORCE :
					send("A3");
					break;
				}


			/***********************************
			The data are sent via a general path
			***********************************/
			supervisor->getMutexCommand()->lock();
			data = command->getData(dataSwitch);
			supervisor->getMutexCommand()->unlock();
		
			dataDelayed = delay(data, dataSwitch, i);
		
			for (int i = 0; i < 3; i++)
			{
				send(boost::lexical_cast<std::string>(dataDelayed[i]));
			}
		}
		usleep( sleepTime );
	}
}