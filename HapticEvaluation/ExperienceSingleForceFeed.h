#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include "ReplWidget.h"
#include "ExperienceWidget.h"

class ExperienceSingleForceFeed : public ExperienceWidget
{
public:
	ExperienceSingleForceFeed(void);
	~ExperienceSingleForceFeed(void);
	void applySettings();
private:
	ReplWidget *rep;
};

