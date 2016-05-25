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
		boost::asio::io_service& io_service, 
		const std::string& host,
		const std::string& port
	);
	~WriteNetworkThread( void );
	void run();
	void send(std::string str);

private :
	
	boost::asio::io_service& io_service_;
	udp::socket socket_;
	udp::endpoint endpoint_;
	Vector3 transA;
	/*
	Vector3 translation;
	char translationCom[3][8]; //to store the data for sending through the socket
	*/
};