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
	
	RemoteControlLaw *command = supervisor->getCommand(); 
	//might be some initialization pb here
	/*nothing to initialize the data from outside*/
	
	//create a new socket for communication
	while(supervisor->getThreadStarted())
	{
		for (int i = 0; i < 4; i++)
		{
			bytes_transferred = socket_.receive_from(boost::asio::buffer(recv_buffer), receiver_endpoint);
			dataType = handle_receive(bytes_transferred);
		}
		
		dataControl.x=data[0];
		dataControl.y=data[1];
		dataControl.z=data[2];

		supervisor->getMutexCommand()->lock();
		command->setData(dataControl, dataType); //Receive data from remote point and set it to HaptdeviceB
		supervisor->getMutexCommand()->unlock();
		
		usleep( sleepTime );
	}
}

DataType ReadNetworkThread::handle_receive(size_t bytes_transferred)
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
				return REMOTE_POSITION;
				break;
			}
			
			else if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("F")+1)
			{
				data_type=2;
				byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
				return REMOTE_FORCE;
				break;
			}
			else if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("V")+1)
			{
				data_type=3;
				byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
				return REMOTE_VELOCITY;
				break;
			}
			else if(std::string(recv_buffer.begin(), recv_buffer.begin()+1).compare("C")+1)
			{
				data_type=4;
				byte_number = atoi(std::string(recv_buffer.begin()+1, recv_buffer.begin()+2).c_str());
				return DESIRED_REMOTE_VELOCITY;
				break;
			}
		}

	//translation data on the 3 axis
	case 1 :
		{
			data[cpt] = boost::lexical_cast<float, std::string>(std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred));
			cpt++;
			if (cpt == byte_number)
			{
				cpt = 0;
				data_type = 0;
			}
			return REMOTE_POSITION;
			break;
		}
	//Force applied to the remote device
	case 2 :
		{
			data[cpt] = boost::lexical_cast<float, std::string>(std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred));
			cpt++;
			if (cpt == byte_number)
			{
				cpt = 0;
				data_type = 0;
			}
			return REMOTE_FORCE;
			break;
		}

		
	//Velocity of the remote device
	case 3 :
		{
			data[cpt] = boost::lexical_cast<float, std::string>(std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred));
			cpt++;
			if (cpt == byte_number)
			{
				cpt = 0;
				data_type = 0;
			}
			return REMOTE_VELOCITY;
			break;
		}
		
	//Desired velocity used to compute the control law for scattering theory
	case 4 :
		{
			data[cpt] = boost::lexical_cast<float, std::string>(std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred));
			cpt++;
			if (cpt == byte_number)
			{
				cpt = 0;
				data_type = 0;
			}
			return DESIRED_REMOTE_VELOCITY;
			break;
		}
	}
};
