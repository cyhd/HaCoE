#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QPushButton>


class AutoWidget : public QWidget
{
public:
	AutoWidget(void);
	~AutoWidget(void);

private:
	QGridLayout *lay;

	//Qlabel
	QLabel *explenation;
	QLabel *lFile;
	QLabel *lIndex;
	QLabel *lSign;
	QLabel *lDirection;
	QLabel *lMagnitude;
	QLabel *lOutputHand;
	QLabel *lOrder;
	QLabel *lDistance;
	QLabel *lDelta;
	QLabel *lForceMin;
	
	//QTexEdit
	QLineEdit *teFile;
	QLineEdit *teGo;

	//QPushButton
	QPushButton *bLoadFile;
	QPushButton *bLoad;
	QPushButton *bGo;
};

