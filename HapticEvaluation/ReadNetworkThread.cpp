#include "ReadNetworkThread.h"
#include "haptlinksupervisor.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
using boost::asio::ip::udp;


ReadNetworkThread::ReadNetworkThread(unsigned short port, int sleep)    
	: io_service(),
      socket_(io_service, udp::endpoint(udp::v4(), port))
{
	sleepTime =  sleep;
	//run();
    io_service.run();
}

ReadNetworkThread::~ReadNetworkThread() 
{
	socket_.close();
}


int ReadNetworkThread::getDataType(size_t bytes_transferred)
{
	if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("T")==0)
	{
		dataType = REMOTE_POSITION;
		byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
	}		
	else if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("F")==0)
	{
		dataType = REMOTE_FORCE;
		byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
	}
	else if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("V")==0)
	{
		dataType = REMOTE_VELOCITY;
		byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
	}
	else if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("P")==0)
		{
		dataType = DESIRED_REMOTE_POSITION;
		byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
	}
	else if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("A")==0)
	{
		dataType = REMOTE_APPLIED_FORCE;
		byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());	
	}
	return byte_number;
}


void ReadNetworkThread::run()
{
	
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	
	RemoteControlLaw *command = supervisor->getCommand(); 
	
	//create a new socket for communication
	while(supervisor->getThreadStarted())
	{
		bytes_transferred = socket_.receive_from(boost::asio::buffer(recv_buffer), receiver_endpoint);
		byte_number = getDataType(bytes_transferred); //Get the type of data to be receive and the number of data to catch
		
		for (int i = 0; i < byte_number; i++)
		{
			bytes_transferred = socket_.receive_from(boost::asio::buffer(recv_buffer), receiver_endpoint);
			dataControl[i]=boost::lexical_cast<float, std::string>(std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred)); //receive the 3 datas to follow
		}
		
		supervisor->getMutexCommand()->lock();
		command->setData(dataControl, dataType);
		supervisor->getMutexCommand()->unlock();
		
		usleep( sleepTime );
	}
}
