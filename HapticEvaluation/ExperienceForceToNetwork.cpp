#include "ExperienceForceToNetwork.h"


ExperienceForceToNetwork::ExperienceForceToNetwork(void)
{
	name = "Force to Network";
	lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Dual Link Force Control through Network:\nThis control scheme uses a master-master configuration.\n-Omni A and B will output a force proportional to the difference in position between them to try and move to the same position.");

	//Rep Widget
	ftn = new ForceToNetworkWidget();

	lay->addWidget(ExperienceWidget::expInfo);
	lay->addWidget(ftn);
	setLayout(lay);
}


ExperienceForceToNetwork::~ExperienceForceToNetwork(void)
{
}



void ExperienceForceToNetwork::applySettings(){
	std::cout<<"ExperienceForceToNetwork:applySettings"<<std::endl;

	// CHOOSE CONTROL MODE
	ControlMode mode;
	if (ftn->rbPositionControl->isChecked())
		mode = SIMPLE_MODE;
	else if(ftn->rbScatteringControl->isChecked())
		mode = SCATTERING_MODE;
	else if(ftn->rbVelocityControl->isChecked()) 
		mode = VELOCITY_MODE;
	else if(ftn->rbWaveControl->isChecked()) 
		mode = WAVE_MODE;
	else if(ftn->rbDelayControl->isChecked()) 
		mode = DELAYED_MODE;
	std::cout<<"ExperienceForceToNetwork:control mode:"<<mode<<std::endl;

	initCmd(mode, ftn->teDelay->text().toInt());

	// External control
	if(ftn->cbExternalControl->isChecked())
	{
		initExternalCmd();
		HaptLinkSupervisor::getInstance()->setExperimentType(FORCE2NET_TEST);
	} else {
		HaptLinkSupervisor::getInstance()->setExperimentType(FORCE2NET);
	}
}


void ExperienceForceToNetwork::initCmd(ControlMode mode, int timeDelay)
{
	//TODO UNCOMMENT THIs
	HaptLinkSupervisor::getInstance()->initCommand(mode, timeDelay);
	DataLogger::getInstance()->setCommandActive(true);
}


void ExperienceForceToNetwork::initExternalCmd()
{
	HaptLinkSupervisor::getInstance()->initExternalCommand();
	DataLogger::getInstance()->setExternalCommandActive(true);
}


	
