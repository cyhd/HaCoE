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

#include "MilliSecTime.h"
#include <cstring>		//for NULL


//private constructor
MilliSecTime::MilliSecTime(void)
{
}

/***************************************/
//Singleton class specific
/***************************************/

//class pointer initialization (obligatory for singleton to compile properly)
MilliSecTime* MilliSecTime::instance = NULL;

//public: returns pointer to singleton class instance
MilliSecTime* MilliSecTime::getInstance()
{
	if (instance == NULL) {
		instance = new MilliSecTime();
	}
	return instance;
}

/***************************************/
//Public
/***************************************/

//Deconstructor
MilliSecTime::~MilliSecTime(void)
{
}

//public method
int MilliSecTime::GetMilliCount()
{
	timeb tb;
	ftime( &tb );
	int nCount=	tb.millitm + (tb.time & 0xfffff) * 1000;

return nCount;
}

//public method
int MilliSecTime::GetMilliSpan()
{
	int nSpan = GetMilliCount() - nTimeStart;
	if (nSpan < 0)
	nSpan += 0x100000 * 1000;

return nSpan;
}

//public method
int MilliSecTime::GetMilliSpan(int StartTime)
{
	int nSpan = GetMilliCount() - StartTime;
	if (nSpan < 0)
	nSpan += 0x100000 * 1000;

return nSpan;
}

//public method
void MilliSecTime::setTimeStart()
{
	nTimeStart = GetMilliCount();
}

int MilliSecTime::getTimeStart()
{
return nTimeStart;
}