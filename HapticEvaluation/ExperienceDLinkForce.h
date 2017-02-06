#pragma once
#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <qtextedit.h>
#include "ExperienceWidget.h"

class ExperienceDLinkForce : public ExperienceWidget
{
public:
	ExperienceDLinkForce();
	~ExperienceDLinkForce(void);
	void applySettings();
};

