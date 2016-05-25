#include "ReadNetworkThread.h"
#include "haptlinksupervisor.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
using boost::asio::ip::udp;


ReadNetworkThread::ReadNetworkThread()    
	: io_service(),
      socket_(io_service, udp::endpoint(udp::v4(), 7171))
{
    data_type=0;
	cpt=0;
	byte_number=0;
	run();
    io_service.run();
};

ReadNetworkThread::~ReadNetworkThread() 
{
	socket_.close();
}

void ReadNetworkThread::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceB = supervisor->getHaptDeviceB();
	
	//create a new socket for communication
	
	while(supervisor->getThreadStarted())
	{
		socket_.async_receive_from(boost::asio::buffer(recv_buffer), receiver_endpoint,
                               boost::bind(&ReadNetworkThread::handle_receive, this,
                               boost::asio::placeholders::error,
                               boost::asio::placeholders::bytes_transferred));
	
		transB.x=trans[0];
		transB.x=trans[1];
		transB.x=trans[2];

		supervisor->getMutex()->lock();
		haptDeviceB->writePosition(transB, rotB); //get data from device A to send to the remote point
		supervisor->getMutex()->unlock();
		
		usleep( sleepTime );
	}

}

void ReadNetworkThread::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
    switch(data_type)
	{
	//décodage du type de données et du nombre de paquets à recevoir
	case 0:
		{
			if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("T")+1)
			{
				data_type=1;
				byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
				break;
			}
			else if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("R")+1)
			{
				data_type=2;
				byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
				break;
			}
		}

	//récupération des données de translation sur les 3 axes
	case 1 :
		{
			if (cpt<byte_number)
			{
				
				trans[cpt] = boost::lexical_cast<float, std::string>(std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred));
				cpt++;
			}
			else
			{
				cpt = 0;
				data_type = 0;
			}
			break;
		}

	//récupération des 4 données de rotation
	case 2 :
		{
			if (cpt<byte_number)
			{
				
				rot[cpt] = boost::lexical_cast<float, std::string>(std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred));
				cpt++;
			}
			else
			{
				cpt = 0;
				data_type = 0;
			}
			break;
		}
	}
    if (!error || error == boost::asio::error::message_size)
        run();
};
