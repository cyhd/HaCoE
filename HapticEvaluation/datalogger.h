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

/**********************************************************************************************************
DataLogger class

Created: March 2010
Author: mr.sc. Boris Takac, dipl.ing

This class is used for writing real-time data from devices to .xml file. Class is implemented as singleton 
and observer. It is attached to HaptLinkSupervisor class subject during single instance construction.
Observer inherited update() function is reimplemented and it executes upon getting notification from 
Supervisor class.

Logging of data can only be done in programm after connection to devices was established in Supervisor class.
Start of logging is calles bz user event on the StartLog button at gui. This button changes the state of 
sessionOpen bool variable. Upon user event for start of logging OpenSessionLog() method is executed.
Then the  m

*************************************************************************************************************/
#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <QXmlStreamWriter>
#include <QFile>
#include "datamodel.h"
#include "Observer.h"
#include <QKeyEvent>

class DataLogger : public Observer
{
public:
	//Inherited and class specific
	static DataLogger* getInstance();	 //returns pointer to singleton class instance
    virtual void update(short value);    //Observer class virtual function
    
	//Log file manipulation
	void OpenSessionLog(QString filename,QString pref);// age,QString sex,QString pref);
	void OpenSessionLog(QString filename,QString pref,QString force,QString forceSign,QString direction,QString trialNumber,QString experimentFile,QString Order);// overloaded for writing a haptic replication experiment trial
	
	void CloseSessionLog();	
	void WritePacket(Vector3 &dataPack, QString element);
	void WritePacket(double xx, QString element);
	QXmlStreamWriter xml;			//main object for holding xml for writing
	void setDataActiveA( bool active ) { dataActiveA = active; }
	void setDataActiveB( bool active ) { dataActiveB = active; }
	void setHapticActiveA( bool active ) { hapticActiveA = active; }
	void setHapticActiveB( bool active ) { hapticActiveB = active; }

	void setCommandActive (bool active) {commandActive = active;}
	void setExternalCommandActive (bool active) {externalCommandActive = active;}

	void setInstruction( bool active ) { instruct = active; }

	void setAkey(int milli) { keyA=milli; }
	void setZkey(int milli) { keyZ=milli; }
	void setLog(bool flag) { logEnabled=flag; }

private:
	bool logEnabled;
	int keyA;
	int keyZ;
	static DataLogger *instance;	//pointeron the class instance 
    DataLogger();					//Constructor
	QFile xmlfile;					//pointer for output file for use of QXmlStreamWriter
	bool dataActiveA;
	bool dataActiveB;

	int milliInit, milliEnd;
	int milliCpt;
	int timeMilli;

	bool commandActive; 
	bool externalCommandActive;
	
	bool hapticActiveA;
	bool hapticActiveB;	
	bool instruct; // fill in parameters
};

#endif // DATALOGGER_H
