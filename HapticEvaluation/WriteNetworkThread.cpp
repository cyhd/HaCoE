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

	//init transA
	transA = Vector3(0.0,0.0,0.0);
	sleepTime = sleep; //sleep = 2ms

	//init the delay
	delayValue = timeDelay/sleepTime+1;
	buffCpt = 0;
	for (int i = 0; i < delayValue; i++)
		transABuff[i]=transA;


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
Vector3 WriteNetworkThread::delay(Vector3 transNew)
{
	transABuff[buffCpt]=transNew;
	buffCpt++;
	if (buffCpt == delayValue)
		buffCpt = 0;
	return transABuff[buffCpt]; 
}

void WriteNetworkThread::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	HapticDevice *haptDeviceA = supervisor->getHaptDeviceA();
	
	while( supervisor->getThreadStarted() )
	{
		
		supervisor->getMutexA()->lock();
		transA = haptDeviceA->getTranslation(); //get data from device A to send to the remote point
		supervisor->getMutexA()->unlock();
		
		transADelayed = delay(transA);

		send("T3"); //code of data T=Translation, 3=3 datas
		send(boost::lexical_cast<std::string>(transADelayed.x));
		send(boost::lexical_cast<std::string>(transADelayed.y));
		send(boost::lexical_cast<std::string>(transADelayed.z));
		
		usleep( sleepTime );
	}
	
}