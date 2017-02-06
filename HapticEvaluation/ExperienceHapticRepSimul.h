#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include "ReplWidget.h"
#include "ExperienceWidget.h"

class ExperienceHapticRepSimul : public ExperienceWidget
{
public:
	ExperienceHapticRepSimul(void);
	~ExperienceHapticRepSimul(void);
	void applySettings();
	outputSide getPref();
	ReplWidget *rep;
};

