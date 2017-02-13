#include "ExperienceSLinkControl.h"


ExperienceSLinkControl::ExperienceSLinkControl()
{
	name = "Single Link Control";
	ExperienceWidget::lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Single Link Position Control:\nThis control scheme uses a master-slave configuration.\n-Entact A is Master, in force mode outputting no force while sending its position to Entact B.\n-Entact B is Slave, reading the position of Entact A and moving to match that position directly.");

	lay->addWidget(ExperienceWidget::expInfo);
	setLayout(lay);
}


ExperienceSLinkControl::~ExperienceSLinkControl(void)
{
}

void ExperienceSLinkControl::applySettings(){
	std::cout<<"ExperienceSLinkControl: APPLY SETTINGS"<<std::endl;
	HaptLinkSupervisor::getInstance()->setExperimentType( SLPOS );

}