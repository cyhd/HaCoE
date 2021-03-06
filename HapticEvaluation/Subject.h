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

#pragma once
#include "Observer.h"

#include <vector>

using std::vector;

class Subject
{

public:
	Subject(void);
	~Subject(void);
	void attachObserver(Observer *obs )  { 
		ObserverList.push_back(obs); }
	void notify(short value=0)
	{
		if (!ObserverList.empty()) 
			for (unsigned int i=0; i<ObserverList.size();i++) // HACK TODO DOES NOT WORK (size goes crazy)
			{
				ObserverList[i]->update(value);
			}
	}
private :
	vector<Observer*> ObserverList;
};
