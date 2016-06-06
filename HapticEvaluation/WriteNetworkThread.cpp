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
		int sleep
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
		sleepTime = sleep;

		//run();
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

void WriteNetworkThread::run()
{
	
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	HapticDevice *haptDeviceA = supervisor->getHaptDeviceA();
	
	while( supervisor->getThreadStarted() )
	{
		
		supervisor->getMutexA()->lock();
		transA = haptDeviceA->getTranslation(); //get data from device A to send to the remote point
		supervisor->getMutexA()->unlock();
		

		send("T3"); //code of data T=Translation, 3=3 datas
		send(boost::lexical_cast<std::string>(transA.x));
		send(boost::lexical_cast<std::string>(transA.y));
		send(boost::lexical_cast<std::string>(transA.z));
		
		usleep( sleepTime );
	}
	
}