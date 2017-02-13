#include "ExperienceNONE.h"


ExperienceNONE::ExperienceNONE(void)
{
	name = "NONE";
	lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("NO EXPERIENCE SELECTED YET.");

	lay->addWidget(ExperienceWidget::expInfo);
	setLayout(lay);
}


ExperienceNONE::~ExperienceNONE(void)
{
}

void ExperienceNONE::applySettings(){
	std::cout<<"ExperienceNONE: APPLY SETTINGS"<<std::endl;
}