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

#include "hrexperiment.h"
#include <sstream>

using namespace std;

HRExperiment::HRExperiment()
{
	this->type = HAPTREPSIM;
	this->magnitude = 0;
	this->distance=0;
	this->delta=0;
	this->forceMin=0;
	this->direction = X_DIRECTION;
	this->sign = POSITIVE;
	this->hand = DOMINANT;
	this->order = HR_SEQUENTIAL;
	this->index = 0;
}

// experiment for Haptic Replication
HRExperiment::HRExperiment(expType type, double magnitude , forceDirection direction , forceSign sign , stimHand hand , HROrder order , int index )
{
	this->type = type;
	this->distance=0;
	this->delta=0;
	this->forceMin=0;
	this->magnitude = magnitude;
	this->direction = direction;
	this->sign = sign;
	this->hand = hand;
	this->order = order;
	this->index = index;
}

HRExperiment::HRExperiment(expType type, double magnitude , double distance, double deltadepth, double forceMin, forceDirection direction , forceSign sign , stimHand hand , HROrder order , int index )
{
	this->type = type;
	this->magnitude = magnitude;
	this->distance = distance;
	this->delta = deltadepth;
	this->forceMin = forceMin;
	this->direction = direction;
	this->sign = sign;
	this->hand = hand;
	this->order = order;
	this->index = index;
}

string HRExperiment::toString(double magnitude) const
{
	stringstream s;
	s << magnitude;
	return s.str();
}

string HRExperiment::toString(forceDirection direction) const
{
	if ( direction == X_DIRECTION )
		return "X_DIRECTION";
	else if ( direction == Z_DIRECTION )
		return "Z_DIRECTION";
	else return "INVALID";
}

string HRExperiment::toString(forceSign sign) const
{
	if ( sign == POSITIVE )
		return "POSITIVE";
	else if ( sign == NEGATIVE )
		return "NEGATIVE";
	else return "INVALID";
}

string HRExperiment::toString(stimHand hand) const
{
	if ( hand == DOMINANT )
		return "DOMINANT";
	else if ( hand == NON_DOMINANT )
		return "NON_DOMINANT";
	else return "INVALID";
}

string HRExperiment::toString(HROrder order) const
{
	if ( order == HR_SIMULTANEOUS )
		return "SIMULTANEOUS";
	else if ( order == HR_SEQUENTIAL )
		return "SEQUENTIAL";
	else return "INVALID";
}