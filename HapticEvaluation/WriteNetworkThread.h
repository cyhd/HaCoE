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
		int sleepTime, 
		int timeDelay
	);
	~WriteNetworkThread( void );
	void run();
	void send(std::string str);
	Vector3 delay(Vector3);

private :

	udp::resolver *resolver;
	udp::resolver::query *query;
	udp::resolver::iterator *iter;
	
	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint endpoint_;
	Vector3 transA;

	//Data for the delay 
	Vector3 transADelayed;
	Vector3 transABuff[1024]; //size of the buffer to be changed with the value in delayValue
	int buffCpt;

	/*************************************************************
	delay is set in sleeptimes. Initiliazed in the constructor.
	The delay is of delay*sleeptimes milliseconds 
	**************************************************************/
	int delayValue; 

	int sleepTime;
};