#include "ExperienceATI.h"


ExperienceATI::ExperienceATI()
{
	lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Use ATI Only: In this mode, only the ATI sensors are used for measuring force.  That is, the haptic devices are not used.");

	lay->addWidget(ExperienceWidget::expInfo);
	setLayout(lay);
}


ExperienceATI::~ExperienceATI(void)
{
}

void ExperienceATI::applySettings(){
	std::cout<<"ExperienceATI::applySettings"<<std::endl;
	HaptLinkSupervisor::getInstance()->setExperimentType( NOHAPTIC );
}