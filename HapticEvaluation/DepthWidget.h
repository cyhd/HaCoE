#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>

class DepthWidget : public QWidget
{
public:
	DepthWidget(void);
	~DepthWidget(void);

	QGridLayout *lay;

	//Qlabel
	QLabel *lDistance;
	QLabel *lDeltaForce;
	QLabel *lDeltaSlope;
	QLabel *lForceMin;
	QLabel *explenation;
	
	//QTexEdit
	QLineEdit *teDistance;
	QLineEdit *teDeltaForce;
	QLineEdit *teDeltaSlope;
	QLineEdit *teForceMin;
};

