#pragma once

#include "datamodel.h"
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
	void handle_receive(size_t);
	
private :

	boost::asio::io_service io_service;
	int data_type, cpt, byte_number;
    udp::socket socket_;
    udp::endpoint receiver_endpoint;
	boost::array<char, 1024> recv_buffer;
	boost::array<float, 3> trans;
	boost::array<float, 4> rot;
	Vector3 transB;
	Matrix3x3 rotB;
	int sleepTime;
	size_t bytes_transferred;
};