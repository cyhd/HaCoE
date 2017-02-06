#pragma once
#include<qlabel.h>
#include <QWidget>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <QRadioButton>
#include <qpushbutton.h>
#include "qcheckbox.h"
#include "qgroupbox.h"

class ForceToNetworkWidget : public QWidget
{
public:
	ForceToNetworkWidget(void);
	~ForceToNetworkWidget(void);

	QGridLayout *lay;

	//Qlabel

	QLabel *lDelay;
	
	//QTexEdit
	QLineEdit *teDelay;

	// QGroupBox
	QGroupBox *gbControlLaw;

	//QRadioButton
	QRadioButton *rbPositionControl;
	QRadioButton *rbVelocityControl;
	QRadioButton *rbScatteringControl;
	QRadioButton *rbWaveControl;
	QRadioButton *rbDelayControl;

	// QCHECKBOX
	QCheckBox *cbExternalControl;

	
};

