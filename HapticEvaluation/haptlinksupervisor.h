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

#ifndef HAPTLINKSUPERVISOR_H
#define HAPTLINKSUPERVISOR_H

//#include "Markup.h"
//#include <afx.h>
#include <QObject>
#include <QTimerEvent>
#include <ctime>
#include "datamodel.h"
#include "Device.h"
#include "Subject.h"
#include "AtiU6.h"
#include "EntactDevice.h"
#include "OmniDevice.h"
#include <QBasicTimer>
#include "HapticThread.h"
#include "HapticThreadDoubleForce.h"
#include "HapticThreadSingleForce.h"
#include "HapticThreadSinglePosition.h"
#include "HapticThreadHaptReplication.h"
#include "HapticThreadSingleHaptic.h"
#include <QMutex>
#include "hrexperiment.h"
#include "experimentreader.h"

enum notifyType 
{
	HR_SEQ_SWITCH,
	HR_SEQ_START,
	HR_SIM_START,
	HR_LOAD_FILE,
	HR_READ_TRIAL,
	HR_GOTO
};


//Start of class definition
class HaptLinkSupervisor : public Subject , public QObject
{
private: 
	double addedForceB; // force added on top by Griffith xp, stored here for logging purpose
	Vector3 positionControlzero;

public:
	//constants
	long counter; //clock counter
	
	//methods
	static HaptLinkSupervisor* getInstance() //returns singleton instance of HaptLinkSupervisor
	{
		if (instance == NULL) 
			instance = new HaptLinkSupervisor();
	
		return instance;
	} 
	~HaptLinkSupervisor();  //destructor
	
	void readForceData(); //reads force data from ATI force sensors connected to LabJack
	//void readHapticData(); //reads position data from Entact.  May also command forces/positions for the ATI to move to/exert
	
	//Vector3 getPosition(){return opti->getTranslation();}
	//Vector3 getOrientation(){return opti->getRotation();}
	Vector3 getForceA() const { return atiA->getTranslation(); } //returns forces from ATI
	Vector3 getTorqueA() const { return atiA->getRotation(); } //returns torques from ATI
	Vector3 getForceB() const { return atiB->getTranslation(); }
	Vector3 getTorqueB() const { return atiB->getRotation(); }
	Vector3 getPositionA() const { return haptDeviceA->getTranslation(); } //returns position data from Entact
	Vector3 getPositionB() const { return haptDeviceB->getTranslation(); }
	Vector3 getOrientationA() const { return haptDeviceA->getRotation(); } //returns rotation data from Entact
	Vector3 getOrientationB() const { return haptDeviceB->getRotation(); }
	bool getThreadStarted() const { return this->threadStarted; } // returns true if a thread has been started
	HapticDevice * getHaptDeviceA() const { return haptDeviceA; } //returns address of haptic devices
	HapticDevice * getHaptDeviceB() const { return haptDeviceB; }
	Vector3 getHaptRepF() const { return haptRepF; } //returns forces and torques set for haptic replication experiment
	Vector3 getHaptRepT() const { return haptRepT; }
	
	outputSide getSideOut() const { return sideOut; } // returns side to output force for haptic replication experiment
	double getK_FORCE() const { return K_FORCE; }
	double getK_TORQUE() const { return K_TORQUE; }
	QMutex * getMutex() const { return mutex; }
	bool getHaptActiveA() const { return haptActiveA; }
	bool getHaptActiveB() const { return haptActiveB; }
	bool getLJActiveA() const { return LJActiveA; }
	bool getLJActiveB() const { return LJActiveB; }
	HRExperiment getExperiment() const { return experiment; }
	expType getExperimentType() { return experimentType; }
	double getForceMin() { return forceMin; }
	double getDistance() { 	return  distance; }
	double getDeltaDepth() { return deltaDepth; }

	// logging
	double getAddedForceB() { return addedForceB; }
	void setAddedForceB(double f) { addedForceB =f; }


	Vector3 getPositionControlzeroB() const { return positionControlzero; }
	void zeroEntactB();
	void calibrate();

	void setLJActiveA( bool active ) { LJActiveA = active; } //Labjack active flags
	void setLJActiveB( bool active ) { LJActiveB = active; }
	void setHaptActiveA( bool active ) { haptActiveA = active; } //Entact active flags
	void setHaptActiveB( bool active ) { haptActiveB = active; }
	void setExperimentType( expType type ) { experimentType = type; } //set up the experiment type
	void setHaptRepF( double Fx , double Fy , double Fz ); //sets forces and torques for haptic replication experiment
	void setHaptRepT( double Tx , double Ty , double Tz );
	void setDepth( double distance , Vector3 force , double slope ); // Griffith force augmentation parameters
	void setSideOut( outputSide side ) { sideOut = side; } //sets side to output force on for haptic replication experiment
	void setK_FORCE( double k ) { K_FORCE = k; }
	void setK_TORQUE( double k ) { K_TORQUE = k; }
	void setHaptRepAutoFilename( QString filename ){ haptRepAutoFilename = filename; }
	void setDominance( outputSide side ) { dominance = side; }
	void setGoToIndex( int index ) { this->indexGoTo = index; }
	void setForceMin(double f) { this->forceMin = f; }
	void setDistance(double d) { this->distance = d; }
	void setDeltaDepth(double d) { this->deltaDepth = d; }

	//time_t getTimeStamp(){return timestamp;}
	int getTimeStamp() { return timestamp; }

	int initDeviceA( char *filename , char *serialNumber ); //initializing Labjacks
	int initDeviceB( char *filename , char *serialNumber );
	int initEntactA( int index , char *ip ); //initializing Entacts
	int initEntactB( int index , char *ip );
	void calibrateEntactA(); //Calibrating entacts
	void calibrateEntactB();

	void timerEvent(QTimerEvent *event); //Timer event handler
	void start(); //starts clocks
	void stop(); //stops clocks
	
	void closeLJConnectionA(); //close Labjack connections
	void closeLJConnectionB();
	void closeEntactConnectionA(); //close Entact connections
	void closeEntactConnectionB();
	void closeConnection(); //this one used at end of program to close all connections
	void resetTimeStamp() { timestamp = 0; }

	virtual void GUINotify( notifyType type );
	
	

protected:
	//void setTimeStamp(time_t tps) { timestamp = tps; }
	void setTimeStamp(int tps) { timestamp = tps; }
		//methods
	static HaptLinkSupervisor *instance;
	HaptLinkSupervisor(QObject* parent = 0) : QObject(parent) //Constructor (private because class is Singleton)
	{ 
		positionControlzero.x=0;
		positionControlzero.y=0;
		positionControlzero.z=0;

		logFlag = 0; 
		threadStarted = false;
		threadCreated = false;
		timerForce = new QBasicTimer();
		mutex = new QMutex();
		dominance = RIGHT;
		experiment = HRExperiment();

		forceMin = 0.0;
		distance = 0.0;
		deltaDepth = 0.0;
	}			

private:  
	Device *atiA;
	Device *atiB;
	HapticDevice *haptDeviceA;
	HapticDevice *haptDeviceB;

	bool LJActiveA;
	bool LJActiveB;
	bool haptActiveA;
	bool haptActiveB;

	expType experimentType;

	//bool timerStarted;
	bool threadStarted;
	bool threadCreated;
	
	QBasicTimer *timerForce;
	HapticThread *thread;
	int logFlag;
	QMutex * mutex;

	//haptic replication preferences
	Vector3 haptRepF;
	Vector3 haptRepT;
	outputSide sideOut;
	double K_FORCE;
	double K_TORQUE;
	//haptic replication parameters from auto generated file
	outputSide dominance;
	QString haptRepAutoFilename;
	double forceMagnitude;
	double forceMin; // set for the purpose of the Griffith XP to set a mininam force feedback
	double distance;
	double deltaDepth;
		

	HRExperiment experiment; // holds an experiment file for automated experiment
	int indexGoTo;
		
	//time_t timestamp;
	int timestamp;

	static const double HR_K_FORCE;
	static const double HR_K_TORQUE;

	
};
#endif // HAPTLINKSUPERVISOR_H
