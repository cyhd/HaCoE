#pragma once

#include "datamodel.h"
#include "HapticThread.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
using boost::asio::ip::udp;


class ReadNetworkThread : public HapticThread
{
public:
	ReadNetworkThread( void );
	~ReadNetworkThread( void );
	void run();
	void handle_receive(const boost::system::error_code& error, size_t);
	
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
	
	/*
	char* packetID;
	char* nbBytes;
	char translationCom[3][8];
	*/
};