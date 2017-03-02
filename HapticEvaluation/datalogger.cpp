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

#include <Windows.h>
//#include <iostream>
#include <stdio.h>
#include <math.h>
#include "datalogger.h"
#include "haptlinksupervisor.h"

bool SIGNAL = true;

//private Constructor
DataLogger::DataLogger()
{
	HaptLinkSupervisor::getInstance()->attachObserver(this);		//attach LOGGER as Oberver to Subject
	dataActiveA = false;
	dataActiveB = false;
	instruct=false;
	logEnabled=true;
	keyA=0;
	keyZ=0;
	milliCpt = 0;
	milliInit = 0;
	milliEnd= 0;
	milliCpt= 0;
	timeMilli= 0;
}


//class pointer initialization
DataLogger* DataLogger::instance = NULL;

//public: returns pointer to class instance
DataLogger* DataLogger::getInstance()
{
	if (instance == NULL) {
		instance = new DataLogger();
	}
	return instance;
}

 
//call based on notification in HaptLinkSupervisors
void DataLogger::update(short value)
{
	if(logEnabled) 
	{
		if (value == HAPTIC_UPDATE_LOG)
		{
			timeMilli = HaptLinkSupervisor::getInstance()->getTimeStamp()-milliInit;
			milliCpt++;
			//HaptLinkSupervisor::getInstance()->getMutexA()->lock();//mutex
			
			xml.writeStartElement( "Data" );
			//bool error = xml.hasError(); //debug
			xml.writeTextElement( "T_ms",QString::number(timeMilli) );
			
			//error = xml.hasError(); //debug
			//WritePacket(HaptLinkSupervisor::getInstance()->getPosition(),"Position_m");
			//WritePacket(HaptLinkSupervisor::getInstance()->getOrientation(),"Orientation_deg");
			
			if ( dataActiveA )
			{
			
				WritePacket( HaptLinkSupervisor::getInstance()->getForceA() , "A_Force_N" );
				WritePacket( HaptLinkSupervisor::getInstance()->getTorqueA() , "A_Torque_Nmm" );
				if (SIGNAL) 
				{
					Vector3 f = HaptLinkSupervisor::getInstance()->getForceA();
					if (fabs(f.z) > 12) 
						//std::cout << '\a'<<std::endl;; 
						Beep( 900, 50 );
				}
			}
			/*
			if ( hapticActiveA )
			{
				//HaptLinkSupervisor::getInstance()->getMutex()->lock();//mutex
				WritePacket(HaptLinkSupervisor::getInstance()->getPositionA() , "A_Position_mm" );
				WritePacket(HaptLinkSupervisor::getInstance()->getOrientationA() , "A_Unit_Vector" );
				//HaptLinkSupervisor::getInstance()->getMutex()->unlock();//end mutex
			}
			*/
			if (commandActive)
			{
				HaptLinkSupervisor::getInstance()->getMutexCommand()->lock(); //mutex
				WritePacket(HaptLinkSupervisor::getInstance()->getCommandForce() , "F_" );
				WritePacket(HaptLinkSupervisor::getInstance()->getCommandPosition() , "P_" );
				WritePacket(HaptLinkSupervisor::getInstance()->getCommandVelocity() , "V_" );
				WritePacket(HaptLinkSupervisor::getInstance()->getCommandRemoteForce() , "FR_" );
				WritePacket(HaptLinkSupervisor::getInstance()->getCommandRemotePosition() , "PR_" );
				WritePacket(HaptLinkSupervisor::getInstance()->getCommandRemoteVelocity() , "VR_" );
				HaptLinkSupervisor::getInstance()->getMutexCommand()->unlock();
			}

			if (externalCommandActive)
			{
				//HaptLinkSupervisor::getInstance()->getMutexCommand()->lock();
				WritePacket(HaptLinkSupervisor::getInstance()->getExternalCommandForce() , "FE_" );
				WritePacket(HaptLinkSupervisor::getInstance()->getExternalCommandPosition() , "PE_" );
				WritePacket(HaptLinkSupervisor::getInstance()->getExternalCommandDesiredPosition() , "DPE_" );
				WritePacket(HaptLinkSupervisor::getInstance()->getExternalCommandVelocity() , "VE_" );
				//HaptLinkSupervisor::getInstance()->getMutexCommand()->unlock();
			}

			if ( dataActiveB )
			{
				WritePacket(HaptLinkSupervisor::getInstance()->getForceB(),"B_Force_N");
				WritePacket(HaptLinkSupervisor::getInstance()->getTorqueB(),"B_Torque_Nmm");
				if (SIGNAL) 
				{
					Vector3 f = HaptLinkSupervisor::getInstance()->getForceB();
					if (fabs(f.z) > 12) 
						//std::cout << '\a'<<std::endl;; 
						Beep( 900, 50 );
				}
			}
			if ( hapticActiveB )
			{
				//HaptLinkSupervisor::getInstance()->getMutex()->lock();//mutex
				WritePacket(HaptLinkSupervisor::getInstance()->getPositionB() , "B_Position_mm" );
				WritePacket(HaptLinkSupervisor::getInstance()->getOrientationB() , "B_Unit_Vector" );
				//HaptLinkSupervisor::getInstance()->getMutex()->unlock();//end mutex
			}
			if (instruct)
			{
				WritePacket(HaptLinkSupervisor::getInstance()->getDistance() , "Distance_mm" );
				WritePacket(HaptLinkSupervisor::getInstance()->getDeltaDepth() , "DeltaDepth_mm" );
				WritePacket(HaptLinkSupervisor::getInstance()->getHaptRepF() , "ForceMaxAdded_N" ); 
				WritePacket(HaptLinkSupervisor::getInstance()->getAddedForceB() , "CurrentForceAdded_N" );
				WritePacket(HaptLinkSupervisor::getInstance()->getForceMin() , "ForceMin_N" );
			
			}
			if (keyA != 0)
			{
				WritePacket(keyA , "KeyPressA" );
				keyA=0;
			}
			if (keyZ != 0)
			{
				WritePacket(keyZ , "KeyPressZ" );
				keyZ=0;
			}
			//error = xml.hasError(); //debug
			xml.writeEndElement();
			
			//HaptLinkSupervisor::getInstance()->getMutexA()->unlock();//end mutex
		}
		if (value == SEQ_SWITCH)
		{
			int militime = HaptLinkSupervisor::getInstance()->getTimeStamp();	
			HaptLinkSupervisor::getInstance()->getMutexA()->lock();//mutex
			DataLogger::getInstance()->xml.writeStartElement("HaptRepSeqSwitch" );
			DataLogger::getInstance()->xml.writeTextElement("SeqSwitch",QString::number(militime));		
			//DataLogger::getInstance()->WritePacket(HaptLinkSupervisor::getInstance()->getPosition(),"Position_m");
			DataLogger::getInstance()->xml.writeEndElement();
			HaptLinkSupervisor::getInstance()->getMutexA()->unlock();//end mutex
		}
	}
}

/***************************************/
//Log file manipulation
/***************************************/
//public method: Opens file for further writing  
//filename = XML filename, age = age of the subject, sex = sex of the subject, pref = left or right handed
//this function is called from GUI
void DataLogger::OpenSessionLog(QString filename, QString pref)//QString age,QString sex,QString pref)
{
	//Date and time information of the XML log file
	time_t TimeStamp;
	time(&TimeStamp);
	//filename.append( QString::QString(ctime(&TimeStamp)));

	xmlfile.setFileName(filename);

    if (!xmlfile.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

	xml.setDevice(&xmlfile);
	xml.writeStartDocument();

	xml.writeStartElement("LogFile");
	xml.writeTextElement("Created",QString::QString(ctime(&TimeStamp)));
	//xml.writeTextElement("Age",age);
	//xml.writeTextElement("Sex",sex);
	xml.writeTextElement("HandDominance",pref);

	milliInit = HaptLinkSupervisor::getInstance()->getTimeStamp();
	xml.writeTextElement("MilliInit",QString::number(milliInit));

	//xml.writeTextElement("HandDominance",QString::number(HaptLinkSupervisor::getInstance()->getCommandForce().x,'f',3));
	//xml.writeEndElement();
}

// TODO: Adapter constructeur pour experience HaptReptAuto
/*
void DataLogger::OpenSessionLog(QString filename,QString pref,QString force,QString forceSign,QString direction,QString trialNumber,QString experimentFile,QString order)//QString age,QString sex,QString pref)
{
	OpenSessionLog(filename,pref);

	HRExperiment experiment = HaptLinkSupervisor::getInstance()->getExperiment();
   QString::fromStdString(experiment.toString( experiment.getMagnitude())) ,
												  QString::fromStdString(experiment.toString( experiment.getSign())) ,
												  QString::fromStdString(experiment.toString( experiment.getDirection())) , 
												  QString::fromStdString(experiment.toString( experiment.getIndex())) , 
												  ui.lineEditExpFile->text() ,
												  QString::fromStdString(experiment.toString( experiment.getOrder())) );
	
	xml.writeTextElement("ExperimentFile",experimentFile);
	xml.writeTextElement("TrialIndex",trialNumber);
	xml.writeTextElement("OutputForce",force);
	xml.writeTextElement("OutputForceSign",forceSign);
	xml.writeTextElement("OutputForceDirection",direction);
	xml.writeTextElement("Order",order);
}*/

//public method: Closes open file after closing all the open keys   
void DataLogger::CloseSessionLog()
{
	milliEnd = HaptLinkSupervisor::getInstance()->getTimeStamp()-milliInit;
	xml.writeTextElement("MilliEnd",QString::number(milliEnd));
	xml.writeTextElement("MilliCpt",QString::number(milliCpt));
	xml.writeEndElement();
	xml.writeEndDocument();
	xmlfile.close();
}

//public method: writes new data from devices to xml file
//compliant with dynamicData struct defined in datamodel.h
void DataLogger::WritePacket(Vector3 &dataPack, QString element)
{
	xml.writeStartElement(element);

	//xml.writeStartElement(element + "X");
	xml.writeTextElement(element + "X",QString::number(dataPack.x,'f',3));
	//xml.writeEndElement();
	
	//xml.writeStartElement(element + "Y");
	xml.writeTextElement(element + "Y",QString::number(dataPack.y,'f',3));
	//xml.writeEndElement();

	//xml.writeStartElement(element + "Z");
	xml.writeTextElement(element + "Z",QString::number(dataPack.z,'f',3));
	//xml.writeEndElement();

	/*
	xml.writeTextElement(element + "Y",QString::number(dataPack.y,'f',3));
	xml.writeTextElement(element + "Z",QString::number(dataPack.z,'f',3));
//	xml.writeTextElement("Total"+element,QString::number( sqrt( pow( dataPack.x , 2 ) + pow( dataPack.y , 2 ) + pow( dataPack.z , 2 ) ) ) );
	*/
	xml.writeEndElement();
}

void DataLogger::WritePacket(double xx, QString element)
{
	xml.writeStartElement(element);
	xml.writeTextElement(element,QString::number(xx,'f',3));
	xml.writeEndElement();
}