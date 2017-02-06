#pragma once

#include <QWidget>
#include "ExperimentInfoWidget.h"
#include <iostream>
#include "haptlinksupervisor.h"
#include "datalogger.h"
#include "Observer.h"

class ExperienceWidget : public QWidget, Observer
{
	Q_OBJECT

public:
	ExperienceWidget(QWidget *parent = NULL);
	~ExperienceWidget(void);
	virtual void applySettings() = 0;
	void update(short value);	//inherited from Observer

protected:
	QGridLayout *lay;
	ExperimentInfoWidget* expInfo;
	QLabel *lExperimentInfo;
};