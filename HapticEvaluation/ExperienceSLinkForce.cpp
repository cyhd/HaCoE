#include "ExperienceSLinkForce.h"


ExperienceSLinkForce::ExperienceSLinkForce()
{
	name = "Single Link Force";
	lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Single Link Force Control: \nThis control scheme uses a master-slave configuration.\n-Entact A is Master, in force mode outputting no force while sending its position to Entact B.\n-Entact B is Slave, reading the position of Entact A and outputting force proportional to difference in position between Entact A and B to move itself to Entact A's position");

	lay->addWidget(ExperienceWidget::expInfo);
	
	setLayout(lay);
}


ExperienceSLinkForce::~ExperienceSLinkForce(void)
{
}
void ExperienceSLinkForce::applySettings(){
	std::cout<<"ExperienceSLinkForce: APPLY SETTINGS"<<std::endl;
	std::cout<<"ExperienceSLinkForce: PREFS IN USE"<<std::endl; /*
	HaptLinkSupervisor::getInstance()->setExperimentType( SINGLEHAPTIC );

		HaptLinkSupervisor::getInstance()->setHaptRepF( ui.lineEditHRFx->text().toDouble() , //Setting up the forces for the experiment.  toFloat() returns 0.0 when conversion fails
														ui.lineEditHRFy->text().toDouble() , //e.g. "2.3" in string returns 2.3, "Spock" in string returns 0.0
														ui.lineEditHRFz->text().toDouble() );
		HaptLinkSupervisor::getInstance()->setHaptRepT( ui.lineEditHRTx->text().toDouble() , 
														ui.lineEditHRTy->text().toDouble() ,
														ui.lineEditHRTz->text().toDouble() );

		HaptLinkSupervisor::getInstance()->GUINotify( HR_SIM_START );
		if ( getPref() == LEFT )
			HaptLinkSupervisor::getInstance()->setSideOut( RIGHT );
		else 
				HaptLinkSupervisor::getInstance()->setSideOut( LEFT );
				*/
}