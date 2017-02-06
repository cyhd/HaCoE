#pragma once
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <qtextedit.h>

#include "ExperienceWidget.h"
class ExperienceSLinkControl : public ExperienceWidget
{
public:
	ExperienceSLinkControl();
	~ExperienceSLinkControl(void);
	void applySettings();
};

