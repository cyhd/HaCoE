#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include "AutoWidget.h"
#include "ExperienceWidget.h"

class ExperienceHapticRepAuto : public ExperienceWidget
{
public:
	ExperienceHapticRepAuto(void);
	~ExperienceHapticRepAuto(void);
	void applySettings();

private:
	AutoWidget *autoW;
};

