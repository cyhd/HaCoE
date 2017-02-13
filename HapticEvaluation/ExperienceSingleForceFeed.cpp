#include "ExperienceSingleForceFeed.h"


ExperienceSingleForceFeed::ExperienceSingleForceFeed(void)
{
	name = "Single Force Feed";
	lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("ExperienceHapticRepAuto");

	//Rep Widget
	rep = new ReplWidget();

	lay->addWidget(ExperienceWidget::expInfo);
	lay->addWidget(rep);
	setLayout(lay);
}


ExperienceSingleForceFeed::~ExperienceSingleForceFeed(void)
{
}

void ExperienceSingleForceFeed::applySettings(){
	std::cout<<"ExperienceSingleForceFeed: APPLY SETTINGS"<<std::endl;
	HaptLinkSupervisor::getInstance()->setExperimentType( SLFORCE );
}