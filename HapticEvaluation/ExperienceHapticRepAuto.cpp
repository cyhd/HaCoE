#include "ExperienceHapticRepAuto.h"


ExperienceHapticRepAuto::ExperienceHapticRepAuto(void)
{
	name = "Haptic Replication Auto";
	lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Haptic Replication Experiment:\nThis experiment tests the ability of a subject to feel a force on one hand and to reproduce it in another.\n-This mode reads from an XML file to automatically run experiments.\n-Use the controls in the frame on the right to operate this mode.\n-Valid file must be input and opened before start button is enabled.");

	//Rep Widget
	autoW = new AutoWidget();

	lay->addWidget(ExperienceWidget::expInfo);
	lay->addWidget(autoW);
	setLayout(lay);
}


ExperienceHapticRepAuto::~ExperienceHapticRepAuto(void)
{
}

void ExperienceHapticRepAuto::applySettings(){
	std::cout<<"ExperienceHapticRepAuto: APPLY SETTINGS"<<std::endl;
	std::cout<<"ExperienceHapticRepAuto: GO SEE CODE: LOGGER DISABLED BECAUSE WEIRD STUFF  HAPPENS"<<std::endl;
	HaptLinkSupervisor::getInstance()->setExperimentType( HAPTREPAUTO );

	HRExperiment experiment = HaptLinkSupervisor::getInstance()->getExperiment();
	
}
