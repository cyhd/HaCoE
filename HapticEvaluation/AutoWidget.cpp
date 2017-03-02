#include "AutoWidget.h"


AutoWidget::AutoWidget(void)
{
	lay = new QGridLayout(this);
	
	// QLabel
	explenation = new QLabel("Auto Haptic Replication Controls");
	lFile = new QLabel("Input File");
	lIndex = new QLabel("Index");
	lSign = new QLabel("Sign");
	lDirection = new QLabel("Direction");
	lMagnitude = new QLabel("Magnitude");
	lOutputHand = new QLabel("Output Hand");
	lOrder = new QLabel("Order");
	lDistance = new QLabel("Distance");
	lDelta = new QLabel("Delta");
	lForceMin = new QLabel("Force Min");

	// QTextEdit
	teFile = new QLineEdit();
	teGo = new QLineEdit();

	//QRadioButton
	bLoadFile = new QPushButton("Load File");
	bLoad = new QPushButton("Load Next Trial");
	bGo = new QPushButton("Go to Trial");


	// Add to layRight Widgets
	lay->addWidget(explenation);
	lay->addWidget(lFile,1,0);
	lay->addWidget(teFile,1,1);
	lay->addWidget(bLoadFile,2,1);
	lay->addWidget(lIndex,3,0);
	lay->addWidget(lSign,4,0);
	lay->addWidget(lDirection,5,0);
	lay->addWidget(lMagnitude,6,0);
	lay->addWidget(lOutputHand,7,0);
	lay->addWidget(lOrder,8,0);
	lay->addWidget(lDistance,9,0);
	lay->addWidget(lDelta,10,0);
	lay->addWidget(lForceMin,11,0);

	lay->addWidget(bLoad,12,0);
	lay->addWidget(bGo,12,1);
	lay->addWidget(teGo,12,2);

	// Add the layRight and layLeft to the parent layout
	setLayout(lay);
}


AutoWidget::~AutoWidget(void)
{
}
