#pragma once

#include "datamodel.h"
#include "RemoteControlLaw.h"
#include <qthread.h>
#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::udp;


class ReadNetworkThread : public QThread
{
public:
	ReadNetworkThread( unsigned short port, int sleepTime );
	~ReadNetworkThread( void );
	void run();
	int getDataType(size_t);
	void handleReceive(size_t, int cpt);
	
private :

	boost::asio::io_service io_service;
	int byte_number;
    udp::socket socket_;
    udp::endpoint receiver_endpoint;
	boost::array<char, 1024> recv_buffer;

	boost::array<float, 3> data;
	Vector3 dataControl;
	DataType dataType;

	int sleepTime;
	size_t bytes_transferred;
};