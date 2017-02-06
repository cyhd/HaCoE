#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include <qgroupbox.h>


class ReplWidget : public QWidget
{
public:
	ReplWidget(void);
	~ReplWidget(void);

	QGridLayout *lay;

	//Qlabel
	QLabel *explenation;
	QLabel *lForce;
	QLabel *lTorque;
	QLabel *lX;
	QLabel *lY;
	QLabel *lZ;
	QLabel *lRadioB;
	
	//QTexEdit
	QLineEdit *teX1;
	QLineEdit *teY1;
	QLineEdit *teZ1;
	QLineEdit *teX2;
	QLineEdit *teY2;
	QLineEdit *teZ2;

	//QRadioButton
	QRadioButton *rbDominant;
	QRadioButton *rbNonDominant;
	QRadioButton *rbPrefLeft;
	QRadioButton *rbPrefRight;

	// QGroupBox
	QGroupBox *gbPrefChoice;

};

