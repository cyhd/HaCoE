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
		boost::asio::io_service& io_service, 
		const std::string& host,
		const std::string& port
	) : io_service_(io_service), socket_(io_service, udp::endpoint(udp::v4(), 0)) {
		udp::resolver resolver(io_service_);
		udp::resolver::query query(udp::v4(), host, port);
		udp::resolver::iterator iter = resolver.resolve(query);
		endpoint_ = *iter;
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
	boost::asio::io_service io_service;
	WriteNetworkThread client(io_service, "localhost", "7171");
	
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();

	while( supervisor->getThreadStarted() )
	{
		supervisor->getMutex()->lock();
		transA = haptDeviceA->getTranslation(); //get data from device A to send to the remote point
		supervisor->getMutex()->unlock();
		
		client.send("T3"); //code of data T=translation, 3=3 datas
		client.send(boost::lexical_cast<std::string>(transA.x));
		client.send(boost::lexical_cast<std::string>(transA.y));
		client.send(boost::lexical_cast<std::string>(transA.z));

		usleep( sleepTime );
	}
}