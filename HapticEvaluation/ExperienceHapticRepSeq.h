#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include "ReplWidget.h"
#include "ExperienceWidget.h"

class ExperienceHapticRepSeq : public ExperienceWidget
{
public:
	ExperienceHapticRepSeq(void);
	~ExperienceHapticRepSeq(void);
	void applySettings();
	outputSide getPref();
	ReplWidget *rep;
};

