#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include "DepthWidget.h"
#include "ExperienceWidget.h"

class ExperienceForceDepthL : public ExperienceWidget
{
public:
	ExperienceForceDepthL(void);
	~ExperienceForceDepthL(void);
	void applySettings();
private:
	DepthWidget *depth;
};

