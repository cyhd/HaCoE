#include "DepthWidget.h"


DepthWidget::DepthWidget(void)
{
	lay = new QGridLayout();
	
	// QLabel
	lDistance = new QLabel("Distance (mm) ");
	lDeltaForce = new QLabel("Delta Force (N)");
	lDeltaSlope = new QLabel("Delta Slope [0-1] ");
	lForceMin = new QLabel("Force min (N)");
	explenation = new QLabel("Preference control on the right says which device is used");

	// QTextEdit
	teDistance = new QLineEdit();
	teDeltaForce = new QLineEdit();
	teDeltaSlope = new QLineEdit();
	teForceMin = new QLineEdit();
	teDistance->setFixedWidth(50);
	teDeltaForce->setFixedWidth(50);
	teDeltaSlope->setFixedWidth(50);
	teForceMin->setFixedWidth(50);


	// Add to layRight Widgets
	lay->addWidget(lDistance,1,1);
	lay->addWidget(lDeltaForce,2,1);
	lay->addWidget(lDeltaSlope,3,1);
	lay->addWidget(lForceMin,4,1);
	lay->addWidget(explenation,5,1);

	lay->addWidget(teDistance,1,2);
	lay->addWidget(teDeltaForce,2,2);
	lay->addWidget(teDeltaSlope,3,2);
	lay->addWidget(teForceMin,5,2);


	// Add the layRight and layLeft to the parent layout
	setLayout(lay);
}


DepthWidget::~DepthWidget(void)
{
}
