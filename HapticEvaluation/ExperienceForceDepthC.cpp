#include "ExperienceForceDepthC.h"


ExperienceForceDepthC::ExperienceForceDepthC(void)
{
	name = "Force depth Constant";
	lay = new QGridLayout(this);

	// Exp info widget
	expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Single Haptic Force Feedback from z=depth:\nThis experiment provides a constant feedback (Force Min + delta Force).\nThe Delta Slope is used here as a measure, delta, in mm of the distance (z-delta/2; z+delta/2) where the system goes to go to Force Min.");

	//Rep Widget
	depth = new DepthWidget();

	lay->addWidget(ExperienceWidget::expInfo);
	lay->addWidget(depth);
	setLayout(lay);
}


ExperienceForceDepthC::~ExperienceForceDepthC(void)
{
}


void ExperienceForceDepthC::applySettings(){
	std::cout<<"ExperienceForceDepthC: APPLY SETTINGS"<<std::endl;
	std::cout<<"ExperienceForceDepthC: GO SEE CODE: PREFS IN USE"<<std::endl;
	
	HaptLinkSupervisor::getInstance()->setExperimentType( DEPTHCONST );
	
	HaptLinkSupervisor::getInstance()->setDistance(depth->teDistance->text().toDouble());
	HaptLinkSupervisor::getInstance()->setDeltaDepth (depth->teDeltaSlope->text().toDouble() );
	HaptLinkSupervisor::getInstance()->setHaptRepF( 0, 0,depth->teDeltaForce->text().toDouble() );
		
	HaptLinkSupervisor::getInstance()->setForceMin(depth->teForceMin->text().toDouble());


	HaptLinkSupervisor::getInstance()->GUINotify( HR_SIM_START ); // Force 0
	// TODO: WHY PREF
	// HaptLinkSupervisor::getInstance()->setSideOut( getPref()  );
		
}