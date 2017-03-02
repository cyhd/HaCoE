#pragma once
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <qtextedit.h>
#include "ExperienceWidget.h"

class ExperienceATI : public ExperienceWidget
{
public:
	ExperienceATI();
	~ExperienceATI(void);
	void applySettings();
};

