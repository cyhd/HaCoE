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
		std::string port
	) : io_service(),
      socket_(io_service, udp::endpoint(udp::v4(), 0))
	{
		io_service.run();
		std::cout<<"initialisation lecture"<<std::endl;
		udp::resolver resolver(io_service);
		udp::resolver::query query((udp::v4(), host, port));
		udp::resolver::iterator iter = resolver.resolve(query);
		endpoint_ = *iter;
		run();
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
		socket_.send_to(boost::asio::buffer(&char_buff[0], char_buff.size()), endpoint_);
	}

void WriteNetworkThread::run()
{
	
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	
	while( supervisor->getThreadStarted() )
	{
		
		supervisor->getMutex()->lock();
		transA = haptDeviceA->getTranslation(); //get data from device A to send to the remote point
		supervisor->getMutex()->unlock();
		
		send("T3"); //code of data T=translation, 3=3 datas
		send(boost::lexical_cast<std::string>(transA.x));
		send(boost::lexical_cast<std::string>(transA.y));
		send(boost::lexical_cast<std::string>(transA.z));
		
		usleep( sleepTime );
	}
	
}