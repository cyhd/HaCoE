#include "ForceToNetworkWidget.h"


ForceToNetworkWidget::ForceToNetworkWidget(void)
{
	lay = new QGridLayout(this);
	
	// QLabel
	lDelay = new QLabel("Delay (ms)");

	// QTextEdit
	teDelay = new QLineEdit();

	//QRadioButton
	gbControlLaw =  new QGroupBox("Control law");
	rbPositionControl = new QRadioButton("Position Control");
	rbVelocityControl = new QRadioButton("Velocity Control");
	rbScatteringControl = new QRadioButton("Scattering Control");
	rbWaveControl = new QRadioButton("Wave Control");
	rbDelayControl = new QRadioButton("Delay Control");

	QVBoxLayout *layControlLawChoice = new QVBoxLayout();
    
    layControlLawChoice->addWidget(rbPositionControl);
	layControlLawChoice->addWidget(rbVelocityControl);
	layControlLawChoice->addWidget(rbScatteringControl);
	layControlLawChoice->addWidget(rbWaveControl);
	layControlLawChoice->addWidget(rbDelayControl);
    gbControlLaw->setLayout(layControlLawChoice);

	// QCheckBox
	cbExternalControl = new QCheckBox("Activate external control");

	// Add to layRight Widgets
	lay->addWidget(gbControlLaw,1,0);
	lay->addWidget(lDelay,7,0);
	lay->addWidget(teDelay,7,1,1,1);
	lay->addWidget(cbExternalControl,9,0);

	// Init default law
	rbPositionControl->setChecked(true);

	// Add the layRight and layLeft to the parent layout
	setLayout(lay);
}


ForceToNetworkWidget::~ForceToNetworkWidget(void)
{
}
