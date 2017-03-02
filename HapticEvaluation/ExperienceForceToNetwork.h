#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include "ForceToNetworkWidget.h"
#include "ExperienceWidget.h"
#include "RemoteControlLaw.h"

class ExperienceForceToNetwork : public ExperienceWidget
{ 
public:
	ExperienceForceToNetwork(void);
	~ExperienceForceToNetwork(void);
	
	// Methods
	void applySettings();
	void initCmd(ControlMode mode, int timeDelay);
	void initExternalCmd();


private:
	ForceToNetworkWidget *ftn;
};

