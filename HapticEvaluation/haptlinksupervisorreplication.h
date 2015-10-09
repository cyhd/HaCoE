#pragma once

#include "haptlinksupervisor.h"


class HaptLinkSupervisorReplication : public HaptLinkSupervisor {
public:
	static HaptLinkSupervisor* getInstance() //returns singleton instance of HaptLinkSupervisor
	{
		if (instance == NULL) 
			instance = new HaptLinkSupervisorReplication();
	
		return instance;
	} 

	virtual void GUINotify( notifyType type );
private:
	//static HaptLinkSupervisorReplication * instance;
	HaptLinkSupervisorReplication(QObject* parent = 0) : HaptLinkSupervisor(parent) {}

};