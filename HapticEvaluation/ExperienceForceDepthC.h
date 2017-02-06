#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include "DepthWidget.h"
#include "ExperienceWidget.h"

class ExperienceForceDepthC : public ExperienceWidget
{
public:
	ExperienceForceDepthC(void);
	~ExperienceForceDepthC(void);
	void applySettings();
private:
	DepthWidget *depth;
};

