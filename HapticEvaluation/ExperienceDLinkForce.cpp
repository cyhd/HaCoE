#include "ExperienceDLinkForce.h"


ExperienceDLinkForce::ExperienceDLinkForce()
{
	name = "Dual Link Force Control";
	ExperienceWidget::lay = new QGridLayout(this);

	// Exp info widget
	expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Dual Link Force Control:\nThis control scheme uses a master-master configuration.\n-Entact A and B will output a force proportional to the difference in position between them to try and move to the same position.");

	lay->addWidget(ExperienceWidget::expInfo);
	setLayout(lay);
}


ExperienceDLinkForce::~ExperienceDLinkForce(void)
{
}


void ExperienceDLinkForce::applySettings(){
	std::cout<<"ExperienceDLinkForce: APPLY SETTINGS"<<std::endl;
	HaptLinkSupervisor::getInstance()->setExperimentType( DLFORCE );
}