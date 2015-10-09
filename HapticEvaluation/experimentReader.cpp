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

#include "experimentReader.h"
#include <QXmlStreamAttributes>

#include <iostream>

ExperimentReader* ExperimentReader::instance = NULL;
//bool ExperimentReader::experimentDone = false;

ExperimentReader* ExperimentReader::getInstance()
{
	if (instance == NULL) {
		instance = new ExperimentReader();	
	}
	return instance;
}

void ExperimentReader::OpenSessionRead(QString filename)
{
	//this->experimentDone = false;
	xmlfile.setFileName( filename );
	if (!xmlfile.open( QIODevice::ReadOnly | QIODevice::Text ))
	{
		this->fileOpened = false;
		return;
	}
	xml.clear(); 
	this->fileOpened = true;
	xml.setDevice(&xmlfile);
	
	while ( readName() != "ExperimentFile" ) //THESE readNextStartElement() calls must be here for readNextExperiment to work.
		xml.readNextStartElement();

	std::cout << "read attr \n" << std::endl;
	QXmlStreamAttributes attr =  xml.attributes();
	if (attr.size() > 0){
		QXmlStreamAttribute at = attr.at(0);
		//QString name = at.name().toString();
		QString value = at.value().toString();

		/*
		SLPOS, // Single Link Position Control
		SLFORCE, // Single Link Force Control
		DLFORCE, // Dual Link Force Control
		HAPTREPSIM, // Haptic Replication Experiment
		HAPTREPSEQ,  // Haptic Replication Experiment
			HAPTREPAUTO,  // forbidden in the XML FILE as already in AUTO MODE !
		NOHAPTIC, // use ATI sensors with no haptic devices
		SINGLEHAPTIC, // Griffith: single device force feedback
		DEPTHCONST, // Griffith: constant force feedback from on z=depth
		DEPTHLINEAR // Griffith: linear force feedback from on z=depth
		*/
		if (value == "HAPTREPSIM"){
			type = HAPTREPSIM;
		}
		if (value == "HAPTREPSEQ"){
			type = HAPTREPSEQ;
		}
		if (value == "DEPTHCONST"){
			type = DEPTHCONST;
		}
		if (value == "DEPTHLINEAR"){
			type = DEPTHLINEAR;
		}


	}
	
	xml.readNextStartElement();
}

void ExperimentReader::CloseSessionRead()
{
	xmlfile.close();
	this->instance = NULL;
	this->getInstance();
}

string ExperimentReader::readElement()
{
	//xml.readNext();
	return xml.readElementText().toStdString();
}

string ExperimentReader::readName()
{
	return xml.name().toString().toStdString();
}

HRExperiment ExperimentReader::readNextTrial()
{
	token = xml.tokenType();
	if ( !xml.isEndElement() && this->fileOpened )
	{
		forceDirection direction;
		forceSign sign;
		HROrder order;
		stimHand hand;
		double magnitude =0;
		double distance =0;
		double deltadepth = 0;
		double fmin = 0;
		int index;

		string str_temp_element;
		string str_temp_name;

		do {
			xml.readNextStartElement();

			str_temp_name = readName();
			str_temp_element = readElement();

			if ( str_temp_name == "Index" )
				index = strToInt( str_temp_element );
			else if ( str_temp_name == "Magnitude" )
				magnitude = strToDouble( str_temp_element );
			else if ( str_temp_name == "Distance" )
				distance = strToDouble( str_temp_element );
			else if ( str_temp_name == "Delta" )
				deltadepth = strToDouble( str_temp_element );
			else if ( str_temp_name == "ForceMin" )
				fmin = strToDouble( str_temp_element );
			else if ( str_temp_name == "Direction" )
			{
				if ( str_temp_element == "X_DIRECTION" )
					direction = X_DIRECTION;
				else if ( str_temp_element == "Z_DIRECTION" )
					direction = Z_DIRECTION;
			}
			else if ( str_temp_name == "Sign" )
			{
				if ( str_temp_element == "POSITIVE" )
					sign = POSITIVE;
				else if ( str_temp_element == "NEGATIVE" )
					sign = NEGATIVE;
			}
			else if ( str_temp_name == "HandOut" )
			{
				if ( str_temp_element == "DOMINANT" )
					hand = DOMINANT;
				else if ( str_temp_element == "NON_DOMINANT" )
					hand = NON_DOMINANT;
			}
			else if ( str_temp_name == "Order" )
			{
				if ( str_temp_element == "SIMULTANEOUS" )
					order = HR_SIMULTANEOUS;
				else if ( str_temp_element == "SEQUENTIAL" )
					order = HR_SEQUENTIAL;
			}
		
		} while ( readName() != "Experiment" );

		if (xml.isEndElement()) xml.readNextStartElement(); //xml.readNext(); //if this is not here then readNextStartElement will reach an End Element and it will die not continue onto the next experiment on the next round.

		if ((type == HAPTREPSIM) || (type == HAPTREPSEQ))
				return HRExperiment(type, magnitude , direction , sign , hand , order , index );	

		if ((type == DEPTHCONST) || (type == DEPTHLINEAR))
				return HRExperiment(type, magnitude , distance, deltadepth, fmin, direction , sign , hand , order , index );

		
	}
	else 
	{
		//experimentDone = true;
		this->xml.clear();
		this->CloseSessionRead();
		return HRExperiment(type, 0 , X_DIRECTION , POSITIVE , DOMINANT , HR_SIMULTANEOUS );
	}
}

double ExperimentReader::strToDouble( string number )
{
	return atof( number.c_str() );
}

int ExperimentReader::strToInt( string number )
{
	return atoi( number.c_str() );
}