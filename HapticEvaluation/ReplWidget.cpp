#include "ReplWidget.h"


ReplWidget::ReplWidget(void)
{
	lay = new QGridLayout(this);
	
	// QLabel
	explenation = new QLabel("Settings for Haptic Replication");
	lForce = new QLabel("Force (N)");
	lTorque = new QLabel("Torque (N.mm)");
	lX = new QLabel("X");
	lY = new QLabel("Y");
	lZ = new QLabel("Z");
	lRadioB = new QLabel("Side to output Force");

	// QTextEdit
	teX1 = new QLineEdit();
	teY1 = new QLineEdit();
	teZ1 = new QLineEdit();
	teX2 = new QLineEdit();
	teY2 = new QLineEdit();
	teZ2 = new QLineEdit();

	//QRadioButton
	rbDominant = new QRadioButton("Dominant");
	rbNonDominant = new QRadioButton("Non Dominant");

	rbPrefLeft = new QRadioButton("Left");
	rbPrefRight = new QRadioButton("Right");
	
	// QGroupBox
	gbPrefChoice = new QGroupBox();
	QVBoxLayout *layPref = new QVBoxLayout();
	layPref->addWidget(rbPrefLeft);
	layPref->addWidget(rbPrefRight);
    gbPrefChoice->setLayout(layPref);

	// Add to layRight Widgets
	lay->addWidget(explenation);
	lay->addWidget(lForce,1,1);
	lay->addWidget(lTorque,1,2);
	lay->addWidget(lX,2,0);
	lay->addWidget(lY,3,0);
	lay->addWidget(lZ,4,0);
	lay->addWidget(lRadioB,5,0);

	lay->addWidget(teX1,2,1);
	lay->addWidget(teY1,3,1);
	lay->addWidget(teZ1,4,1);
	lay->addWidget(teX2,2,2);
	lay->addWidget(teY2,3,2);
	lay->addWidget(teZ2,4,2);

	lay->addWidget(rbDominant,6,0);
	lay->addWidget(rbNonDominant,7,0);

	lay->addWidget(gbPrefChoice, 8, 0);


	// Add the layRight and layLeft to the parent layout
	setLayout(lay);
}


ReplWidget::~ReplWidget(void)
{
}
