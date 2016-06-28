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
	Vector3 delay(Vector3 data, DataType type, int cpt);

private :

	udp::resolver *resolver;
	udp::resolver::query *query;
	udp::resolver::iterator *iter;
	
	boost::asio::io_service io_service;
	udp::socket socket_;
	udp::endpoint endpoint_;

	Vector3 data;
	Vector3 dataDelayed;
	Vector3 dataBuff[1024][10];
	

	//Data for the delay 
	int buffCpt;
	DataType dataSwitch;
	int dataNumber;
	/*************************************************************
	delay is set in sleeptimes. Initiliazed in the constructor.
	The delay is of delay*sleeptime milliseconds 
	**************************************************************/
	int delayValue; 

	int sleepTime;
};