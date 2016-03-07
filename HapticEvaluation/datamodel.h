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

#ifndef DATAMODEL_H
#define DATAMODEL_H

//constants

enum UPDATE_FLAG
{
	HAPTIC_UPDATE_LOG,
	HAPTIC_UPDATE_GUI,
	HR_FILE_OPEN_SUCCESS,
	HR_FILE_OPEN_FAIL,
	HR_FILE_END,
	HR_PARAMETER_UPDATE,
	SEQ_SWITCH
};
//const int HAPTIC_UPDATE_LOG = 1;
//const int HAPTIC_UPDATE_GUI = 2;

const int SAMPLE_RATE = 60;
const int HAPTIC_LOOP_RATE = 3;
const int HAPTIC_GUI_RATE = 4;
const int HAPTIC_LOG_RATE = 1;

const int DURATION = 100;
const int MILI_OFFSET = 3;
const int PERIOD = (SAMPLE_RATE+MILI_OFFSET)*HAPTIC_GUI_RATE;

enum outputSide
{
	LEFT,
	RIGHT
};

enum expType
{
	SLPOS, // Single Link Position Control
	SLFORCE, // Single Link Force Control
	DLFORCE, // Dual Link Force Control
	FORCE2NET, //send (and receive) data to network
	HAPTREPSIM, // Haptic Replication Experiment
	HAPTREPSEQ,
	HAPTREPAUTO,
	NOHAPTIC, // use ATI sensors with no haptic devices
	SINGLEHAPTIC, // Griffith: single device force feedback
	DEPTHCONST, // Griffith: constant force feedback from on z=depth
	DEPTHLINEAR // Griffith: linear force feedback from on z=depth
};

struct Vector3 {
	
	Vector3(double xx, double yy, double zz) { x=xx; y=yy; z=zz; }
	Vector3() { x=0; y=0; z=0; }

	double x;
	double y;
	double z;
};

struct Matrix3x3 {

	double mat11;
	double mat12;
	double mat13;
	double mat21;
	double mat22;
	double mat23;
	double mat31;
	double mat32;
	double mat33;
};

#endif // DATAMODEL_H
