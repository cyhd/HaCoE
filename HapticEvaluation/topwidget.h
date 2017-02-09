#ifndef TOPWIDGET_H
#define TOPWIDGET_H

#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QComboBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLineEdit>
#include <iostream>
#include "newwindow.h"

class NewWindow;
class TopWidget : public QWidget
{
	Q_OBJECT

public:
	TopWidget(NewWindow *parent = NULL);
	~TopWidget(void);

	NewWindow *newWindow;
	
	// ComboBox
	QComboBox *cbExperience;

	// Methods
	void initUI();
	void setStatus(char *s); 

	// UI STUFF
	QGridLayout *lay;

	// Labels
	QLabel *lbExperience;
	QLabel *lbIP1;
	QLabel *lbIP2;
	QLabel *lbPort1;
	QLabel *lbPort2;
	QLabel *lbLogFile;
	QLabel *lbDelay;

	// RadioButtons
	QRadioButton *rbRobot1;
	QRadioButton *rbRobot2;
	QRadioButton *rbUser1;
	QRadioButton *rbUser2;

	// QTextEdit
	QLineEdit *teIP1;
	QLineEdit *teIP2;
	QLineEdit *tePort1;
	QLineEdit *tePort2;
	QLineEdit *teLogFile;
	QLineEdit *teDelay;

	// QGroupBox
	QGroupBox *gbRobot;
	QGroupBox *gbUserChoice;
	QGroupBox *gbConfig;
	QGroupBox *gbLogFile;
	QGroupBox *gbExperience;

	// Methods
	void resetUI();
	
	void enableGbRobotType();
	void enableGbLog();
	void disableGbRobotType();
	void disableGbUserChoice();
	void disableGbConfig();
	void disableGbLogFile();
	void enableGbConfig();
	void enableActivate();
	void disableButtons();


public slots:
	void experienceChosen(int);
	void enableGbUserChoice();
	void enableConfigAndButtons();
	void disableCenterWidget();
	void enableCenterWidget();
};

#endif












