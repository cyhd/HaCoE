#pragma once

#include <qthread.h>
#include "HapticThread.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::udp;

class WriteNetworkThread : public HapticThread
{
public:
	WriteNetworkThread(
		std::string host,
		std::string port
	);
	~WriteNetworkThread( void );
	void run();
	void send(std::string str);

private :
	
	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint endpoint_;
	Vector3 transA;
};