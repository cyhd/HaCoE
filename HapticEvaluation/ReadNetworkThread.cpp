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
    data_type=0;
	cpt=0;
	byte_number=0;

	//init trans and rot
	transB = Vector3(0.0,0.0,0.0);

	rotB.mat11 = 0.0;
	rotB.mat12 = 0.0;
	rotB.mat13 = 0.0;
	rotB.mat21 = 0.0;
	rotB.mat22 = 0.0;
	rotB.mat23 = 0.0;
	rotB.mat31 = 0.0;
	rotB.mat32 = 0.0;
	rotB.mat33 = 0.0;

	//run();
    io_service.run();
};

ReadNetworkThread::~ReadNetworkThread() 
{
	socket_.close();
}

void ReadNetworkThread::run()
{
	
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	HapticDevice *haptDeviceB = supervisor->getHaptDeviceB();
	
	supervisor->getMutexA()->lock();
	transB = haptDeviceB->getTranslation(); //get data from device A to send to the remote point
	supervisor->getMutexA()->unlock();

	trans[0]=transB.x;
	trans[1]=transB.y;
	trans[2]=transB.z;

	//create a new socket for communication
	while(supervisor->getThreadStarted())
	{
		bytes_transferred = socket_.receive_from(boost::asio::buffer(recv_buffer), receiver_endpoint);
		handle_receive(bytes_transferred);

		transB.x=trans[0];
		transB.y=trans[1];
		transB.z=trans[2];

		
		supervisor->getMutexB()->lock();
		haptDeviceB->writePosition(transB, rotB); //Receive data from remote point and set it to HaptdeviceB
		supervisor->getMutexB()->unlock();
		

		usleep( sleepTime/2 );
	}
	
}

void ReadNetworkThread::handle_receive(size_t bytes_transferred)
{
    switch(data_type)
	{
	//getting type and number of data to receive
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

	//translation data on the 3 axis
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

	//translation data on the 3 axis
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
};
