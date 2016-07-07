//
// HaCoE Haptic Communication Evaluation is a software to build haptic communication experiments, using haptic arms and force sensors, generating standard XML files you can load in R.
//
//   Copyright (C) 2014 Cedric Dumas 
//
//   contributors: Cedric Dumas, Boris Takac, Devin Luu, Amine Chellali, Caroline Cao
//   contact: Cedric.Dumas@gmail.com
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//

#include "haptlinksupervisor.h"
#include "AtiU6.h"
//#include "OptiTrack.h"
#include "MilliSecTime.h"
#include <iostream>
#include "qthread.h"
#include "HapticThreadSingleHapticCst.h"
#include "HapticThreadSingleHapticSlope.h"
#include "datalogger.h"
#include "HapticThreadForceToNetwork.h"

#include "RemoteControlLawSimple.h"
#include "RemoteControlLawVelocity.h"
#include "RemoteControlLawScatteringTheory.h"
#include "RemoteControlLawWaveTheory.h"
#include "RemoteControlLawDelayed.h"
#include "RemoteControlLawPosition.h"
#include "ExternalControl.h"

#include <windows.h>

using namespace std;

const double HaptLinkSupervisor::HR_K_FORCE = 0.075; //0.075 N/mm
const double HaptLinkSupervisor::HR_K_TORQUE = 100;  //100N.mm/rad approximately (using small angle approximation)  Not Used
HaptLinkSupervisor *HaptLinkSupervisor::instance = NULL;

QMutex * mutexA = new QMutex();
QMutex * mutexB = new QMutex();
QMutex * mutexCommand = new QMutex();

int HaptLinkSupervisor::initDeviceA(char *filename , char *serialNumber) 
{
	atiA = new AtiU6(filename , serialNumber);
	return atiA->getConnectSuccess();
}

int HaptLinkSupervisor::initDeviceB(char *filename , char *serialNumber)
{
	atiB = new AtiU6(filename , serialNumber);
	return atiB->getConnectSuccess();
}

int HaptLinkSupervisor::initHapticA( int index , char *ip )
{
	//haptDeviceA = new EntactDevice( index , ip );

	haptDeviceA = new OmniDevice(index);
	
	return haptDeviceA->getConnectSuccess();
}

int HaptLinkSupervisor::initHapticB( int index , char *ip )
{
	//haptDeviceB = new EntactDevice( index , ip );
	haptDeviceB = new OmniDevice(index);
	
	return haptDeviceB->getConnectSuccess();
}

void HaptLinkSupervisor::initCommand(ControlMode mode, int timeDelay)
{
	switch(mode)
	{
	case SIMPLE_MODE :
		command = new RemoteControlLawSimple();
		break;	
	case POSITION_MODE :
		command = new RemoteControlLawPosition();
		break;
	case SCATTERING_MODE :
		command = new RemoteControlLawScatteringTheory();
		break;
	case VELOCITY_MODE :
		command = new RemoteControlLawVelocity();
		break;
	case WAVE_MODE : 
		command = new RemoteControlLawWaveTheory();
		break;
	case DELAYED_MODE :
		command = new RemoteControlLawDelayed(timeDelay);
		break;
	}
	DataLogger::getInstance()->setLog(true);
}
void HaptLinkSupervisor::initExternalCommand()
{
	externalCommand = new ExternalControl();
}

void HaptLinkSupervisor::calibrateHapticA()
{
	if ( haptActiveA )
		haptDeviceA->calibrate();
}

void HaptLinkSupervisor::calibrateHapticB()
{
	if ( haptActiveB )
		haptDeviceB->calibrate();
}

void HaptLinkSupervisor::zeroEntactB()
{
	if ( haptActiveB ) 
	{
		Vector3 translationDamping, rotationDamping;

		const double HR_GENERAL_DAMPING_TRANSLATION = 0.0008; //damping constants
		const double HR_GENERAL_DAMPING_ROTATION = 1.0;

		translationDamping.x = HR_GENERAL_DAMPING_TRANSLATION; //damping constants for the Entact
		translationDamping.y = HR_GENERAL_DAMPING_TRANSLATION;
		translationDamping.z = HR_GENERAL_DAMPING_TRANSLATION;
		rotationDamping.x = HR_GENERAL_DAMPING_ROTATION;
		rotationDamping.y = HR_GENERAL_DAMPING_ROTATION;
		rotationDamping.z = HR_GENERAL_DAMPING_ROTATION;

		haptDeviceB->setMode( FORCECONTROL_MODE ); //B in Force Control. 
		haptDeviceB->writeDamping( translationDamping , rotationDamping ); //sets damping to the device
	
	// HOME to the start position
		Vector3 zero;
		zero.x = 0;
		zero.y = 0;
		zero.z = 0;
		
		Vector3 un;
		un.x = 0;
		un.y = 0;
		un.z = 0.3;

		haptDeviceB->writeForce( un , zero );
		getMutexB()->lock();
		positionControlzero = haptDeviceB->getTranslation();
		getMutexB()->unlock();
	}
}
/*
//public method for setting up and sustaining device connection and managing data logging
int HaptLinkSupervisor::SuperviseConnection(char *filename , char *serialNumber , 
											char *filename2 , char *serialNumber2)  
{
	//Create new instance for ATI sensor and OptiTrack sensor
	//AtiDummy and OptiTrackDummy are dummy class which allows execution of the program without any actual hardware connected to the PC

	//ati = new AtiDummy(filename);
	//opti = new OptiTrackDummy();

	if (haptActiveA)
	{
		atiA = new AtiU6(filename,serialNumber);
	}
	else atiA = NULL;
		
	if (haptActiveB)
	{
		atiB = new AtiU6(filename2,serialNumber2);
	}
	else atiB = NULL;
	
	//ati = new Ati(filename);
	//opti = new OptiTrack();
	
	return 0;
}
*/
HaptLinkSupervisor::~HaptLinkSupervisor()
{
	closeConnection();
	//delete timerForce;
	if ( threadCreated ) delete thread;
}	


void HaptLinkSupervisor::closeConnection()
{
	if ( LJActiveA == 1 ) 
		closeLJConnectionA();
		
	if ( LJActiveB == 1 )
		closeLJConnectionB();
	
	if ( haptActiveA == 1 )
		closeHapticConnectionA();

	if ( haptActiveB == 1 )
		closeHapticConnectionB();
}

void HaptLinkSupervisor::closeLJConnectionA()
{
	if ( LJActiveA ) 
	{
		atiA->closeConnection();
		delete atiA;
		LJActiveA=false;
	}
}

void HaptLinkSupervisor::closeLJConnectionB()
{
	if ( LJActiveB )
	{
		atiB->closeConnection();
		delete atiB;
		LJActiveB = false;
	}
}
void HaptLinkSupervisor::closeHapticConnectionA()
{
	if ( haptActiveA )
	{
		haptDeviceA->closeConnection();
		delete haptDeviceA;
		haptActiveA = false;
	}
}
void HaptLinkSupervisor::closeHapticConnectionB()
{
	if ( haptActiveB )
	{
		haptDeviceB->closeConnection();
		delete haptDeviceB;
		haptActiveB = false;
	}
}
//timer functions
void HaptLinkSupervisor::start() 
{ 
	if ( experimentType == SLPOS )
	{
		thread = new HapticThreadSinglePosition();
		threadCreated = true;
	}
	else if ( experimentType == SLFORCE )
	{
		thread = new HapticThreadSingleForce();
		threadCreated = true;
	}
	else if ( experimentType == DLFORCE )
	{
		thread = new HapticThreadDoubleForce();
		threadCreated = true;
	}
	else if ( experimentType == FORCE2NET )
	{
		thread = new HapticThreadForceToNetwork(0);
		threadCreated = true;
	}
	else if ( experimentType == FORCE2NET_TEST )
	{
		thread = new HapticThreadForceToNetwork(1);
		threadCreated = true;
	}
	else if (experimentType == SINGLEHAPTIC) 
	{
		thread = new HapticThreadSingleHaptic();
		threadCreated = true;
	}
	else if ( experimentType == DEPTHCONST) 
	{
		thread = new HapticThreadSingleHapticCst();
		threadCreated = true;
	}
	else if ( experimentType == DEPTHLINEAR) 
	{
		thread = new HapticThreadSingleHapticSlope();
		threadCreated = true;
	}
	else if ( experimentType == HAPTREPSIM )
	{
		thread = new HapticThreadHaptReplication();
		threadCreated = true;
	}
	else if ( experimentType == HAPTREPSEQ )
	{
		thread = new HapticThreadHaptReplication();
		threadCreated = true;
	}
	else if ( experimentType == HAPTREPAUTO )
	{
		forceMagnitude = experiment.getMagnitude();
		if ( experiment.getSign() == NEGATIVE ) 
			forceMagnitude *= -1;

		if ( experiment.getDirection() == X_DIRECTION )
			setHaptRepF( forceMagnitude , 0 , 0 );
		else if ( experiment.getDirection() == Z_DIRECTION )
			setHaptRepF( 0 , 0 , forceMagnitude );

		setHaptRepT( 0 , 0 , 0 ); //for these experiments torque is always zero

		distance = experiment.getDistance();
		deltaDepth = experiment.getDeltaDepth();
		forceMin = experiment.getForceMin();

		if ( experiment.getHand() == DOMINANT )
		{
			if ( dominance == RIGHT )
				setSideOut( RIGHT );
			else if ( dominance == LEFT )
				setSideOut( LEFT );
		}
		else if ( experiment.getHand() == NON_DOMINANT )
		{
			if ( dominance == RIGHT )
				setSideOut( LEFT );
			else if ( dominance == LEFT )
				setSideOut( RIGHT );
		}

		if ( experiment.getOrder() == HR_SIMULTANEOUS )
		{
			experimentType = HAPTREPSIM;
			this->GUINotify( HR_SIM_START );
		}
		else if ( experiment.getOrder() == HR_SEQUENTIAL )
		{
			experimentType = HAPTREPSEQ;
			this->GUINotify( HR_SEQ_START );
		}
		
		/*
			SLPOS, // Single Link Position Control
			SLFORCE, // Single Link Force Control
			DLFORCE, // Dual Link Force Control
			HAPTREPSIM, // Haptic Replication Experiment
			HAPTREPSEQ, // Haptic Replication Experiment
			HAPTREPAUTO,
			NOHAPTIC, // use ATI sensors with no haptic devices
			SINGLEHAPTIC, // Griffith: single device force feedback
			DEPTHCONST, // Griffith: constant force feedback from on z=depth
			DEPTHLINEAR // Griffith: 
		*/
		if ((experiment.getExperimentType() ==  HAPTREPSIM)||(experiment.getExperimentType() ==  HAPTREPSEQ))
			thread = new HapticThreadHaptReplication();
		else if (experiment.getExperimentType() ==  DEPTHCONST)
			thread = new HapticThreadSingleHapticCst();
		else if (experiment.getExperimentType() ==  DEPTHLINEAR)
			thread = new HapticThreadSingleHapticSlope();

		threadCreated = true;
	}
	else if ( experimentType == NOHAPTIC )
	{
		threadCreated = false;
		thread = NULL;
	}

	if ( threadCreated )
	{
		thread->start( QThread::HighestPriority );
		threadStarted = true;
	}
	//timerForce->start( SAMPLE_RATE , this ); 
}

void HaptLinkSupervisor::calibrate()  {

	if ( haptActiveB ) {
		getMutexB()->lock();
		positionControlzero = haptDeviceB->getTranslation();
		getMutexB()->unlock();

		DataLogger::getInstance()->setLog(true);
	}
}

void HaptLinkSupervisor::stop() 
{ 
	//timerForce.stop();
	threadStarted = false;

	if ( haptActiveA )
		haptDeviceA->setMode( DISABLED_MODE );
	//if ( haptActiveB )
		//haptDeviceB->setMode( DISABLED_MODE );
}

void HaptLinkSupervisor::timerEvent(QTimerEvent *event) 
{ 
	this->readForceData();
}

void HaptLinkSupervisor::readForceData()
{
	//this function is executed at period = SAMPLE_RATE
	
	//Start and store the initial value of the timer
	if(logFlag == 0)
	{
		 logFlag = 1;
		 MilliSecTime::getInstance()->setTimeStart();
	}
	
	//record the time for XML, difference between each time instants is equal to SAMPLE_RATE
	this->setTimeStamp(MilliSecTime::getInstance()->GetMilliSpan());
	
	
	DataLogger::getInstance()->update(HAPTIC_UPDATE_LOG);
	
	/* TODO got disabled because notify does not work
	//get value from the sensors
	//opti->readData();
	if ( LJActiveA )
		atiA->readData();
	if ( LJActiveB )
		atiB->readData();	
	//if ( haptActiveA )
	//	haptDeviceA->readData();
	//if ( haptActiveB )
	//	haptDeviceB->readData();
	
	if ( counter % HAPTIC_GUI_RATE == 0 )
	{
		counter = 0;
		 this->notify( HAPTIC_UPDATE_GUI );
	}

	//notify datalogger and hapticevaluationgui based on different time instances
	if( counter % HAPTIC_LOG_RATE == 0 )
	{	 
		this->notify( HAPTIC_UPDATE_LOG );
	}
	counter++;
	*/


}

void HaptLinkSupervisor::setHaptRepF( double Fx , double Fy , double Fz )
{
	haptRepF.x = Fx;
	haptRepF.y = Fy;
	haptRepF.z = Fz;
}

void HaptLinkSupervisor::setHaptRepT( double Tx , double Ty , double Tz )
{
	haptRepT.x = Tx;
	haptRepT.y = Ty;
	haptRepT.z = Tz;
}

void HaptLinkSupervisor::setDepth( double distance , Vector3 force , double delta )
{
	this->distance = distance;
	this->haptRepF = force;
	this->deltaDepth = delta;
}


void HaptLinkSupervisor::GUINotify( notifyType type )
{
	if ( type == HR_SEQ_SWITCH ) //gradually turn on the force on the left hand so it does not startle the subject
	{
		if ( experimentType == HAPTREPSEQ )
		{
			for( int i = 0 ; i < 101 ; i++ )
			{
				this->setK_FORCE( i * HR_K_FORCE/100 );
				this->setK_TORQUE( i * HR_K_TORQUE/100 );
				Sleep(7);
			}
			this->setHaptRepF( 0 , 0 , 0 );
			this->setHaptRepT( 0 , 0 , 0 );	
			this->notify( SEQ_SWITCH );
		}
	}
	else if ( type == HR_SEQ_START ) //No force on replicating hand when starting
	{
		this->setK_FORCE( 0 );
		this->setK_TORQUE( 0 );
	}
	else if ( type == HR_SIM_START )
	{
		this->setK_FORCE( HR_K_FORCE );
		this->setK_TORQUE( HR_K_TORQUE );
	}
	else if ( type == HR_LOAD_FILE )
	{
		ExperimentReader::getInstance()->OpenSessionRead( this->haptRepAutoFilename );
		if ( ExperimentReader::getInstance()->getFileOpened() )
			this->notify( HR_FILE_OPEN_SUCCESS );
		else if ( !(ExperimentReader::getInstance()->getFileOpened()) )
			this->notify( HR_FILE_OPEN_FAIL );
	}
	else if ( type == HR_READ_TRIAL )
	{
		this->experiment = ExperimentReader::getInstance()->readNextTrial();
		//if ( ExperimentReader::getInstance()->getExperimentDone() ) //Broken for now
		//{
		//	this->notify( HR_FILE_END );
		//	return;
		//}
		this->notify( HR_PARAMETER_UPDATE );
	}
	else if ( type == HR_GOTO )
	{
		bool isLooped = false;
		if ( this->indexGoTo > 0 )
		{
			//ExperimentReader::getInstance()->OpenSessionRead( this->haptRepAutoFilename );
			while ( this->experiment.getIndex() != this->indexGoTo )
			{
				if ( isLooped && this->experiment.getIndex() == -1 ) //if the reader reaches the end and this has looped from beginning already then
				{
					ExperimentReader::getInstance()->OpenSessionRead( this->haptRepAutoFilename );
					break;
				}
				
				if ( !isLooped && this->experiment.getIndex() == -1 ) //if reader has reached the end and has not looped, then go back to the beginning
				{
					isLooped = true;
					ExperimentReader::getInstance()->OpenSessionRead( this->haptRepAutoFilename );
				}
				this->experiment = ExperimentReader::getInstance()->readNextTrial();
				
				//if ( ExperimentReader::getInstance()->getExperimentDone() ) return;
			}
			this->notify( HR_PARAMETER_UPDATE );
		}
		else return;
	}
}

/** code for the Network thread

*/

void HaptLinkSupervisor::initUDPReadWrite(unsigned short portREAD, std::string ip, std::string portWRITE, int timeDelay)
{
	((HapticThreadForceToNetwork*)thread)->initUDPReadWrite(  portREAD, ip, portWRITE, timeDelay);
		
}

