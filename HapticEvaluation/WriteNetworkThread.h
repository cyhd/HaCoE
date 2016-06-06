#pragma once

#include <qthread.h>
#include "datalogger.h"

#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::udp;

class WriteNetworkThread : public QThread
{
public:
	WriteNetworkThread(
		std::string host,
		std::string port,
		int sleepTime
	);
	~WriteNetworkThread( void );
	void run();
	void send(std::string str);

private :

	udp::resolver *resolver;
	udp::resolver::query *query;
	udp::resolver::iterator *iter;
	
	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint endpoint_;
	Vector3 transA;
	int sleepTime;
};