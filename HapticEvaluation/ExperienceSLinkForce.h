#pragma once
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <qtextedit.h>
#include "ExperienceWidget.h"

class ExperienceSLinkForce : public ExperienceWidget 
{
public:
	ExperienceSLinkForce();
	~ExperienceSLinkForce(void);
	void applySettings();
};

