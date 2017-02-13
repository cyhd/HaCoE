#include "ExperienceHapticRepSeq.h"


ExperienceHapticRepSeq::ExperienceHapticRepSeq(void)
{
	name = "Haptic Replication Sequential";
	lay = new QGridLayout(this);

	// Exp info widget
	ExperienceWidget::expInfo = new ExperimentInfoWidget();
	expInfo->teDetails->setText("Haptic Replication Experiment:\nThis experiment tests the ability of a subject to feel a force on one hand and to reproduce it in another.\n-The test subject will feel a force on Entact B, which he/she will need to reproduce.\n-The test subject will reproduce the force on Entact A, which will be stationary in force mode.");

	//Rep Widget
	rep = new ReplWidget();

	lay->addWidget(ExperienceWidget::expInfo);
	lay->addWidget(rep);
	setLayout(lay);
}


ExperienceHapticRepSeq::~ExperienceHapticRepSeq(void)
{
}

void ExperienceHapticRepSeq::applySettings(){
	std::cout<<"ExperienceHapticRepSeq: APPLY SETTINGS"<<std::endl;
	std::cout<<"GO SEE CODE: PREFERENCES ARE USED HERE."<<std::endl;
	
	HaptLinkSupervisor::getInstance()->setExperimentType( HAPTREPSEQ );
	// TODO: Fix this
	/*
		HaptLinkSupervisor::getInstance()->setHaptRepF( ui.lineEditHRFx->text().toFloat() , //Setting up the forces for the experiment.  toFloat() returns 0.0 when conversion fails
														ui.lineEditHRFy->text().toFloat() , //e.g. "2.3" in string returns 2.3, "Spock" in string returns 0.0
														ui.lineEditHRFz->text().toFloat() );
		HaptLinkSupervisor::getInstance()->setHaptRepT( ui.lineEditHRTx->text().toFloat() , 
														ui.lineEditHRTy->text().toFloat() ,
														ui.lineEditHRTz->text().toFloat() );
		HaptLinkSupervisor::getInstance()->GUINotify( HR_SEQ_START );
		if ( ui.radioButtonSideDom->isChecked() )
			HaptLinkSupervisor::getInstance()->setSideOut( getPref() );
		else if ( ui.radioButtonSideNonDom->isChecked() )
		{
			if ( getPref() == LEFT )
				HaptLinkSupervisor::getInstance()->setSideOut( RIGHT );
			else 
				HaptLinkSupervisor::getInstance()->setSideOut( LEFT );
		}
		*/
		
}

outputSide ExperienceHapticRepSeq::getPref(){
	if (rep->rbPrefRight->isChecked())
		return RIGHT;
	else
		return LEFT;
}
