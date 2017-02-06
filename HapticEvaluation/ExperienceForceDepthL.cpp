#include "ExperienceForceDepthL.h"


ExperienceForceDepthL::ExperienceForceDepthL(void)
{
	lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Single Haptic Force Feedback from z=depth:\nThis experiment provides a linear force feedback (ForceMin + linear force= slope*distance.");

	//Rep Widget
	depth = new DepthWidget();

	lay->addWidget(ExperienceWidget::expInfo);
	lay->addWidget(depth);
	setLayout(lay);
}


ExperienceForceDepthL::~ExperienceForceDepthL(void)
{
}


void ExperienceForceDepthL::applySettings(){
	std::cout<<"ExperienceForceDepthL: APPLY SETTINGS"<<std::endl;
	std::cout<<"ExperienceForceDepthL: PREFS IN USE GO SEE CODE MODAFAKA"<<std::endl;
	
	HaptLinkSupervisor::getInstance()->setExperimentType( DEPTHLINEAR );
		
	HaptLinkSupervisor::getInstance()->setDistance(depth->teDistance->text().toDouble());
	HaptLinkSupervisor::getInstance()->setDeltaDepth (depth->teDeltaSlope->text().toDouble() );
	HaptLinkSupervisor::getInstance()->setHaptRepF( 0, 0,depth->teDeltaForce->text().toDouble() );
		
	HaptLinkSupervisor::getInstance()->setForceMin(depth->teForceMin->text().toDouble());
	HaptLinkSupervisor::getInstance()->GUINotify( HR_SIM_START ); // Force 0
	// TODO: WHY U USE PREF ??
	// HaptLinkSupervisor::getInstance()->setSideOut( getPref()  );
		
}