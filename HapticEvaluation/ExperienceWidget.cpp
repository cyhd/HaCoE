#include "ExperienceWidget.h"


ExperienceWidget::ExperienceWidget(QWidget *parent)
{
	HaptLinkSupervisor::getInstance()->attachObserver(this);
}


ExperienceWidget::~ExperienceWidget(void)
{
}

void ExperienceWidget::update(short value)
{
	/*
	if(value == HAPTIC_UPDATE_GUI)
	{
		HaptLinkSupervisor *sp = HaptLinkSupervisor::getInstance();

		//RemoteControlLaw *command = sp->getCommand();
		//RemoteControlLaw *externalCommand = sp->getExternalCommand();
		
		forceActiveA = getAtiAActivate(); //getting status of activation of each ATI
		forceActiveB = getAtiBActivate();

		//DataLogger::getInstance()->setActiveA(activeA);
		//DataLogger::getInstance()->setActiveB(activeB);
		
		Vector3 empty; //empty vector for the NULL given below
		empty.x = NULL;
		empty.y = NULL;
		empty.z = NULL;

		if ( forceActiveA && forceActiveB )//Only getTorque or getForce if there is sensor connected.  If not then give NULL
		{
			updateDevForceDisplay(sp->getForceA() , sp->getForceB());//sp->getForceA() , sp->getForceB());
			updateDevTorqueDisplay(sp->getTorqueA() , sp->getTorqueB());//sp->getTorqueA() , sp->getTorqueB());
			updateDevGraphDisplay(sp->getForceA() , sp->getForceB());
		}

		else if ( forceActiveA && !forceActiveB )
		{
			updateDevForceDisplay(sp->getForceA() , empty);
			updateDevTorqueDisplay(sp->getTorqueA() , empty);
			updateDevGraphDisplay(sp->getForceA() , empty);
		}

		else if ( !forceActiveA && forceActiveB )
		{
			updateDevForceDisplay(empty , sp->getForceB());
			updateDevTorqueDisplay(empty , sp->getTorqueB());
			updateDevGraphDisplay(empty , sp->getForceB());
		}

		else if ( !forceActiveA && !forceActiveB )
		{
			updateDevForceDisplay(empty , empty);
			updateDevTorqueDisplay(empty , empty);
			updateDevGraphDisplay(empty , empty);
		}
		
		if(ui.frame_6->isVisible())
		{
			updateDevPositionDisplay(sp->getCommandPosition(),empty);
			updateDevForceDisplay(sp->getCommandForce(),empty);
			if(ui.checkBoxFzA_2->isChecked())
				updateDevGraphDisplay(sp->getCommandPosition(),empty);
			else if (ui.checkBoxFxB_2->isChecked())
				updateDevGraphDisplay(sp->getCommandForce(),empty);
		}

		if ((sp->getExperimentType() == DEPTHCONST)||(sp->getExperimentType() == DEPTHLINEAR))
			updateGriffith(sp->getPositionB(), Vector3(0,0,sp->getAddedForceB()));

		//updateDevPositionDisplay(sp->getPosition());
		//updateDevOrientationDisplay(sp->getOrientation());
		//updateDevGraphDisplay(sp->getForceA()) , sp->getForceB());
	    updateCheckGraph();
	}

	else if ( value == HR_FILE_OPEN_SUCCESS )
	{
		setStatus("Haptic replication experiment file successfully opened.");
		ui.pushButtonReadNextTrial->setEnabled( true );
		ui.pushButtonGoTo->setEnabled( true );
		ui.pushButtonStart->setEnabled( true );
		ui.pushButtonZero->setEnabled( false );
	}
	else if ( value == HR_FILE_OPEN_FAIL )
	{
		setStatus("Haptic replication experiment file failed to open.");
		ui.pushButtonReadNextTrial->setEnabled( false );
		ui.pushButtonGoTo->setEnabled( false );
		ui.pushButtonStart->setEnabled( false );
		ui.pushButtonZero->setEnabled( false );
	}
	//else if ( value == HR_FILE_END ) //Broken for now
	//{
	//	setStatus("Haptic replication experiment complete!");
	//	ui.pushButtonReadNextTrial->setEnabled( false );
	//	//ui.pushButtonGoTo->setEnabled( false );
	//	ui.pushButtonStart->setEnabled( false );
	//}
	else if ( value == HR_PARAMETER_UPDATE )
	{
		setHRParameterDisplay();
	}
	*/
}
