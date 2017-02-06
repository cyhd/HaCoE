#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#pragma once

#include <QWidget>
#include <QLabel>
#include <QGridLayout>
#include <QLineEdit>
#include <qtextedit.h>

class CenterWidget : public QWidget
{

public:
	CenterWidget(QWidget *parent = NULL);
	~CenterWidget(void);
	
protected:
	QGridLayout *lay;
	QTextEdit *teExpDetails;
	QLabel *lExperimentInfo;
	QWidget *rightPanel;
};

#endif