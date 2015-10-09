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
HRExperiment class

Created: May 2013
Author: Devin Luu, student ict, CSIRO

This class contains all parameters for a single Haptic Replication Experiment.
It defines:
force magnitude
force direction ( x or z )
force direction ( + or - )
force sequence (sequential or simultaneous)
force presentation side (dominant or non-dominant side)

For a full haptic replication experiment, one object per trial will be created.  
They can be put in a vector and then randomized.
*************************************************************************************************************/
#pragma once
#include <String>
#include "datamodel.h"

using namespace std;


enum forceDirection
{
	X_DIRECTION,
	Z_DIRECTION
};

enum forceSign
{
	POSITIVE,
	NEGATIVE
};

enum HROrder
{
	HR_SIMULTANEOUS,
	HR_SEQUENTIAL
};

enum stimHand
{
	DOMINANT,
	NON_DOMINANT
};

class HRExperiment
{
public:
	HRExperiment(); // default and overloaded constructors.  Default experiment outputs no force and has index 0

	// constructor for replication experiment
	HRExperiment( expType type, double magnitude , forceDirection direction ,
				  forceSign sign , stimHand hand , HROrder order , int index=-1 ); //overloaded constructor.  This is the one that should be used in experiments

	// constructor for haptic augmentation (cst and linear)
	HRExperiment(expType type, double magnitude , double distance, double delta, double forceMin, forceDirection direction , forceSign sign , stimHand hand , HROrder order , int index );

	~HRExperiment(){}

	double getMagnitude() const { return this->magnitude; }  //getters
	forceDirection getDirection() const { return this->direction; }
	forceSign getSign() const { return this->sign; }
	stimHand getHand() const { return this->hand; }
	HROrder getOrder() const { return this->order; }
	int getIndex() const { return this->index; }
	double getDistance() const { return this->distance; }
	expType getExperimentType() const { return this->type; }
	double getDeltaDepth() const { return this->delta; }
	double getForceMin() const { return this->forceMin; }

	//no setters.  Values should not be changed after constructing

	//toString methods are for converting member variables into strings for recording in a file
	string toString( double magnitude ) const;
	string toString( forceDirection direction ) const;
	string toString( forceSign sign ) const;
	string toString( stimHand hand ) const;
	string toString( HROrder order ) const;
	
private:
	expType type;
	double magnitude;
	double distance;
	double delta;
	double forceMin;
	forceDirection direction;
	forceSign sign;
	stimHand hand;
	HROrder order;
	int index;
};
