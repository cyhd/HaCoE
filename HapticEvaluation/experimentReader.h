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

/*********************************************************************************************************
ExperimentReader class

created: 2013-06-12
by: Devin Luu, Student ICT, CSIRO

This class is singleton.  This class is responsible for parsing an XML file with a randomized list of 
haptic replication experiments.

IMPORTANT:
The XML file format must be consistent with the one created using HRExperimentWriter or else this class will not work!

Upon starting a program, one instance of the ExperimentReader class will be instantiated.
After an instance is created, the user will call OpenSessionRead(QString filename) to open a file for reading.

The main functionality for this class is implemented in readNextExperiment()
Each time readNextExperiment() is called, it will read one Haptic Replication Experiment.
readNextExperiment() returns one HRExperiment object each time it is called.  This object contains parameters
for running a haptic replication experiment.

When the end of the file is reached, an readNextExperiment will signal end of file.

To close a file, call CloseSessionRead()
*********************************************************************************************************/
#include <QXmlStreamReader>
#include <QFile>
#include <string>
#include "HRExperiment.h"
#include "datamodel.h"

#pragma once
using namespace std;

class ExperimentReader
{
public:
	static ExperimentReader* getInstance(); //returns an instance of experimentreader
	~ExperimentReader(){};
	void OpenSessionRead(QString filename); //opens session for reading
	void CloseSessionRead(); //close the file after reading is over
	void prepareDocument();
	
	HRExperiment readNextTrial(); //user calls this function to read an experiment
	//bool getExperimentDone() const { return this->experimentDone; }
	bool getFileOpened() const { return this->fileOpened; }
	
	
private:
	ExperimentReader()// private constructor
	{ 
//		this->experimentDone = false; 
		this->fileOpened = false;
		this->type=HAPTREPSIM;
	} 

	QFile xmlfile;
	QXmlStreamReader xml;
	QXmlStreamReader::TokenType token; //reads token type to see what kind of xml element it is currently reading
	static ExperimentReader *instance;
	//static bool experimentDone; //experiment is done when the last trial is reached in the file
	bool fileOpened; 

	string readElement(); //reads an XML element and returns a string for comparison within the class
	string readName(); //reads XML tag name and returns a string for comparison within the class

	double strToDouble( string number ); //
	int strToInt( string number );
	
	
	expType type;
	
};