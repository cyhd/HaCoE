//
// HaCoE Haptic Communication Evaluation is a software to build haptic communication experiments, using haptic arms and force sensors, generating standard XML files you can load in R.
//
//   Copyright (C) 2014 Cedric Dumas 
//
//   contributors: Cedric Dumas, Boris Takac, Devin Luu, Amine Chellali, Caroline Cao
//   contact: Cedric.Dumas@gmail.com
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//

//#include "hapticevaluationgui.h"
//#include "ui_hapticevaluationgui.h"
#include "haptlinksupervisor.h"
#include <QPushButton>
#include <math.h>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QMessageBox>
#include <QFileDialog>
#include <QGroupBox>
#include <qwt_picker.h>
#include "datalogger.h"
#include "qcheckbox.h"
#include "hrexperiment.h"
#include <string>
#include "qwt_compat.h"

using namespace std;
#define KEEP true
HapticEvaluationGUI* HapticEvaluationGUI::instance = NULL;

HapticEvaluationGUI::HapticEvaluationGUI(QWidget *parent, Qt::WindowFlags flags): QMainWindow(parent, flags)
{	
	
	ui.setupUi(this);
	ui.frame_5->setVisible(true); //graph 1
	ui.frame_6->setVisible(false); //graph 2
	initGraph();
	
	prefgroup = new QButtonGroup;
	prefgroup->addButton(ui.radioButtonRight);
	prefgroup->addButton(ui.radioButtonLeft);
	ui.radioButtonRight->setChecked(true);

	expgroup = new QButtonGroup;
	expgroup->addButton(ui.radioButtonSLPosition);
	
	//*******
	expgroup->addButton(ui.radioButtonForceToNet);
	
	expgroup->addButton(ui.radioButtonSLForce);
	expgroup->addButton(ui.radioButtonDLForce);
	expgroup->addButton(ui.radioButtonHaptRep);
	expgroup->addButton(ui.radioButtonATIOnly);
	expgroup->addButton(ui.radioButtonHRSeq);
	expgroup->addButton(ui.radioButtonHaptRepAuto);
	expgroup->addButton(ui.radioButtonSingleHaptic);
	expgroup->addButton(ui.radioButtonDepthConstant);
	expgroup->addButton(ui.radioButtonDepthLinear);


	cmdgroup = new QButtonGroup;
	cmdgroup->addButton(ui.radioButtonPosition);
	cmdgroup->addButton(ui.radioButtonScattering);
	cmdgroup->addButton(ui.radioButtonVelocity);
	cmdgroup->addButton(ui.radioButtonWave);
	cmdgroup->addButton(ui.radioButtonDelayed);
	
	

	//ui.radioButtonDepthConstant->setChecked(true);
	ui.radioButtonATIOnly->setChecked(true);

	ui.textEditExpInfo->setPlainText( "Use ATI Only:\n" 
									  "In this mode, only the ATI sensors are used for measuring force.  That is, the haptic devices are not used." );

	haptrepoutputsidegroup = new QButtonGroup;
	haptrepoutputsidegroup->addButton(ui.radioButtonSideDom);
	haptrepoutputsidegroup->addButton(ui.radioButtonSideNonDom);
	ui.radioButtonSideDom->setChecked(true);

	ui.textEditExpInfo->setReadOnly(true);

	ui.pushButtonStop->setEnabled( false );
	ui.pushButtonZero->setEnabled( false );
	ui.pushButtonGoTo->setEnabled( false );

	disableHRFrame(); //disabling hapt replication settings
	disableDepthFrame();

	ui.pushButtonEntactACalibrate->setEnabled( false );
	ui.pushButtonEntactBCalibrate->setEnabled( false );
	ui.pushButtonComInit->setEnabled( false );

	hideDataA();
	hideDataA_2();

	hideDataB();
	hideDataB_2();

	ui.frameHaptRep->setVisible( false );
	ui.frameRemCtrl->setVisible( false );
	ui.frameHaptRepAuto->setVisible( false );
	ui.pushButtonReadNextTrial->setEnabled( false );
	

	setStatus("Please insert the Log File name, LabJack serial number(s), Entact IPs, and select an experiment then click the Start button to start logging");

	
	connect(ui.checkBoxAtiActivateA , SIGNAL(stateChanged(int)) , this , SLOT(switchAtiA()));
	connect(ui.checkBoxAtiActivateB , SIGNAL(stateChanged(int)) , this , SLOT(switchAtiB()));
	connect(ui.checkBoxActiveEntactA , SIGNAL(stateChanged(int)) , this , SLOT(switchEntactA()));
	connect(ui.checkBoxActiveEntactB , SIGNAL(stateChanged(int)) , this , SLOT(switchEntactB()));


	connect(ui.pushButtonEntactACalibrate , SIGNAL(clicked()) , this , SLOT(calibrateEntactA()));
	connect(ui.pushButtonEntactBCalibrate , SIGNAL(clicked()) , this , SLOT(calibrateEntactB()));
	connect(ui.radioButtonSLPosition , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	
	//**********
	connect(ui.radioButtonForceToNet , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	
	connect(ui.radioButtonSLForce , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	connect(ui.radioButtonDLForce , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	connect(ui.radioButtonHaptRep , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	connect(ui.radioButtonSingleHaptic , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	connect(ui.radioButtonDepthConstant , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	connect(ui.radioButtonDepthLinear , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));

	connect(ui.radioButtonATIOnly , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	connect(ui.radioButtonHRSeq , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));

	// automated test
	connect(ui.radioButtonHaptRepAuto , SIGNAL(toggled(bool)) , this , SLOT(setExpInfo()));
	connect(ui.pushButtonLoadExpFile , SIGNAL(clicked()) , this , SLOT(setHRExpFile()));
	connect(ui.pushButtonReadNextTrial , SIGNAL(clicked()) , this , SLOT(readNextHRTrial()));
	connect(ui.pushButtonGoTo , SIGNAL(clicked()) , this , SLOT(setGoTo()));

	// dominant hand choice
	connect(ui.radioButtonLeft , SIGNAL(toggled(bool)) , this , SLOT(setDominantHand()));
	connect(ui.radioButtonRight , SIGNAL(toggled(bool)) , this , SLOT(setDominantHand()));

	//graph display choice
	connect(ui.pushButtonDisplaySwitch,SIGNAL(clicked()) , this , SLOT(switchDisplay()));
	

	forceActiveA = false;
	forceActiveB = false;


	dateTime = new QDateTime();
	//date = dateTime->currentDateTime().toString( "yyyyMMdd_hhmmss" );
	//ui.lineEditLog->setText( date );

}

HapticEvaluationGUI::~HapticEvaluationGUI()
{
	//delete sexgroup;
	delete prefgroup;
	delete expgroup;
	delete haptrepoutputsidegroup;
	delete dateTime;
	delete zoomer;
	delete [] dataX;
	delete [] dataFxA;
	delete [] dataFyA;
	delete [] dataFzA;
	delete [] dataFtotalA;
	delete [] dataFxB;
	delete [] dataFyB;
	delete [] dataFzB;
	delete [] dataFtotalB;
	delete courbeFxA;
	delete courbeFyA;
	delete courbeFzA;
	delete courbeFtotalA;
	delete courbeFxB;
	delete courbeFyB;
	delete courbeFzB;
	delete courbeFtotalB;
	delete marker;
	delete Grid;
}

void HapticEvaluationGUI::zeroA() { }

void HapticEvaluationGUI::zeroB() {

	ui.pushButtonZero->setEnabled(false);
	HaptLinkSupervisor::getInstance()->calibrate();

}


bool HapticEvaluationGUI::getAtiAActivate()
{
	return ui.checkBoxAtiActivateA->isChecked();
}

bool HapticEvaluationGUI::getAtiBActivate()
{
	return ui.checkBoxAtiActivateB->isChecked();
}

bool HapticEvaluationGUI::getEntactAActivate()
{
	return ui.checkBoxActiveEntactA->isChecked() || ui.checkBoxActiveHapticA->isChecked();
}	
bool HapticEvaluationGUI::getEntactBActivate()
{
	return ui.checkBoxActiveEntactB->isChecked() ;
}

bool HapticEvaluationGUI::getHapticActivate()
{
	return ui.checkBoxActiveHapticA->isChecked();
}

void HapticEvaluationGUI::updateCheckGraph()
 {
	if (ui.frame_5->isVisible())
	{
		state = ui.checkBoxFxA->isChecked();
		if(state)
			courbeFxA->setVisible(true);
		else
			courbeFxA->setVisible(false);

	
		state =ui.checkBoxFyA->isChecked();
		if(state)
			courbeFyA->setVisible(true);
		else 
			courbeFyA->setVisible(false);


		state = ui.checkBoxFzA->isChecked();
		if(state)
			courbeFzA->setVisible(true);
		else
			courbeFzA->setVisible(false);

		state = ui.checkBoxFtotalA->isChecked();
		if(state)
			courbeFtotalA->setVisible(true);
		else
			courbeFtotalA->setVisible(false);

		state = ui.checkBoxFxB->isChecked();
		if(state)
			courbeFxB->setVisible(true);
		else
			courbeFxB->setVisible(false);

	
		state =ui.checkBoxFyB->isChecked();
		if(state)
			courbeFyB->setVisible(true);
		else 
			courbeFyB->setVisible(false);


		state = ui.checkBoxFzB->isChecked();
		if(state)
			courbeFzB->setVisible(true);
		else
			courbeFzB->setVisible(false);

		state = ui.checkBoxFtotalB->isChecked();
		if(state)
			courbeFtotalB->setVisible(true);
		else
			courbeFtotalB->setVisible(false);
		/*
		state = ui.checkBoxPx->isChecked();
		if(state)
			courbePx->setVisible(true);
		else
			courbePx->setVisible(false);
		*/
	}
	else if(ui.frame_6->isVisible())
	{
		
		state = ui.checkBoxFxB_2->isChecked()||ui.checkBoxFzA_2->isChecked();
		if(state)
		{
			courbeFxA->setVisible(true);
			courbeFyA->setVisible(true);
			courbeFzA->setVisible(true);
			courbeFxB->setVisible(true);
			courbeFyB->setVisible(true);
			courbeFzB->setVisible(true);
		}
		else
		{
			courbeFxA->setVisible(false);
			courbeFyA->setVisible(false);
			courbeFzA->setVisible(false);
			courbeFxB->setVisible(false);
			courbeFyB->setVisible(false);
			courbeFzB->setVisible(false);
		}
	}
}
void HapticEvaluationGUI::updateDevForceDisplay(Vector3 f1 , Vector3 f2)
{
	if(ui.frame_5->isVisible())
	{
		QString Fx_A = QString::number(f1.x,'f',3);
		ui.labelFx_A->setText(Fx_A);

		QString Fy_A = QString::number(f1.y,'f',3);
		ui.labelFy_A->setText(Fy_A);

		QString Fz_A = QString::number(f1.z,'f',3);	
		ui.labelFz_A->setText(Fz_A);

		QString Fx_B = QString::number(f2.x,'f',3);
		ui.labelFx_B->setText(Fx_B);

		QString Fy_B = QString::number(f2.y,'f',3);
		ui.labelFy_B->setText(Fy_B);

		QString Fz_B = QString::number(f2.z,'f',3);	
		ui.labelFz_B->setText(Fz_B);
	}
	else if (ui.frame_5->isVisible())
	{
		QString Fx_A = QString::number(f1.x,'f',3);
		ui.labelFx_A_2->setText(Fx_A);

		QString Fy_A = QString::number(f1.y,'f',3);
		ui.labelFy_A_2->setText(Fy_A);

		QString Fz_A = QString::number(f1.z,'f',3);	
		ui.labelFz_A_2->setText(Fz_A);

		QString Fx_B = QString::number(f2.x,'f',3);
		ui.labelFx_B_2->setText(Fx_B);

		QString Fy_B = QString::number(f2.y,'f',3);
		ui.labelFy_B_2->setText(Fy_B);

		QString Fz_B = QString::number(f2.z,'f',3);	
		ui.labelFz_B_2->setText(Fz_B);
	}
}

void HapticEvaluationGUI::updateDevPositionDisplay(Vector3 p, Vector3 g)
{
	
	QString Px_A = QString::number(p.x,'f',3);
	ui.labelTx_A_2->setText(Px_A);
		
	QString Py_A = QString::number(p.y,'f',3);
	ui.labelTy_A_2->setText(Py_A);
    
	QString Pz_A = QString::number(p.z,'f',3);
	ui.labelTz_A_2->setText(Pz_A);
	
	QString Px_B = QString::number(g.x,'f',3);
	ui.labelTx_B_2->setText(Px_A);
		
	QString Py_B = QString::number(g.y,'f',3);
	ui.labelTy_B_2->setText(Py_B);
    
	QString Pz_B = QString::number(g.z,'f',3);
	ui.labelTz_B_2->setText(Pz_B);

}


void HapticEvaluationGUI::updateDevTorqueDisplay(Vector3 t1 , Vector3 t2)
{
	QString Tx_A = QString::number(t1.x,'f',3);
	ui.labelTx_A->setText(Tx_A);

	QString Ty_A = QString::number(t1.y,'f',3);
	ui.labelTy_A->setText(Ty_A);

	QString Tz_A = QString::number(t1.z,'f',3);
	ui.labelTz_A->setText(Tz_A);

	QString Tx_B = QString::number(t2.x,'f',3);
	ui.labelTx_B->setText(Tx_B);

	QString Ty_B = QString::number(t2.y,'f',3);
	ui.labelTy_B->setText(Ty_B);

	QString Tz_B = QString::number(t2.z,'f',3);
	ui.labelTz_B->setText(Tz_B);
}


HapticEvaluationGUI* HapticEvaluationGUI::getInstance()
{
	if(instance == NULL){
		instance = new HapticEvaluationGUI();
	}
	return instance;
}

void HapticEvaluationGUI::switchAtiA()
{
	if (getAtiAActivate())
	{
		if ( HaptLinkSupervisor::getInstance()->initDeviceA( "FT9498.cal" , ui.lineEditLJSerialA->text().toLocal8Bit().data() ) == 1 )
		{
			setStatus("LabJack A activated.  Press Start to start logging.");
			DataLogger::getInstance()->setDataActiveA( true );
			HaptLinkSupervisor::getInstance()->setLJActiveA( true );
			showDataA();
		}
		else 
		{
			setStatus("LabJack A connection failed.  Please check serial number is correct and LabJack is connected.  Then recheck the activate box.");
			ui.checkBoxAtiActivateA->setChecked( false );
			ui.checkBoxAtiActivateA->setCheckState( Qt::Unchecked );
			DataLogger::getInstance()->setDataActiveA( false );
			HaptLinkSupervisor::getInstance()->setLJActiveA( false );
			hideDataA();
		}
	}
	else if ( HaptLinkSupervisor::getInstance()->getLJActiveA() )
	{
		HaptLinkSupervisor::getInstance()->closeLJConnectionA();
		setStatus("LabJack A deactivated.  Press Start to start logging.");
		DataLogger::getInstance()->setDataActiveA( getAtiAActivate() );
		HaptLinkSupervisor::getInstance()->setLJActiveA( getAtiAActivate() );
		hideDataA();
	}
}

void HapticEvaluationGUI::switchAtiB()
{
	if (getAtiBActivate())
	{
		if ( HaptLinkSupervisor::getInstance()->initDeviceB( "FT13441.cal" , ui.lineEditLJSerialB->text().toLocal8Bit().data() ) == 1 )
		{
			setStatus("LabJack B activated.  Press Start to start logging.");
			DataLogger::getInstance()->setDataActiveB( true );
			HaptLinkSupervisor::getInstance()->setLJActiveB( true );
			showDataB();
		}
		else 
		{
			setStatus("LabJack B connection failed.  Please check serial number is correct and LabJack is connected.  Then recheck the activate box.");
			ui.checkBoxAtiActivateB->setChecked( false );
			ui.checkBoxAtiActivateB->setCheckState( Qt::Unchecked );
			DataLogger::getInstance()->setDataActiveB( false );
			HaptLinkSupervisor::getInstance()->setLJActiveB( false );
			hideDataB();
		}
	}	
	else if ( HaptLinkSupervisor::getInstance()->getLJActiveB() )
	{
		HaptLinkSupervisor::getInstance()->closeLJConnectionB();
		setStatus("LabJack B deactivated.  Press Start to start logging.");
		DataLogger::getInstance()->setDataActiveB( getAtiBActivate() );
		HaptLinkSupervisor::getInstance()->setLJActiveB( getAtiBActivate() );
		hideDataB();
	}
}

// ALREADY COPIED
void HapticEvaluationGUI::switchEntactA()
{
	if ( getEntactAActivate() )

	{
		if ( HaptLinkSupervisor::getInstance()->initHapticA( ui.tabWidgetDevice->currentIndex() , ui.lineEditEntactAIP->text().toLocal8Bit().data() ) == 1 )
		{
			setStatus("Haptic device A activated.  Press Start to start logging.");
			DataLogger::getInstance()->setHapticActiveA( true );
			HaptLinkSupervisor::getInstance()->setHaptActiveA( true );
			ui.pushButtonEntactACalibrate->setEnabled( true );
			ui.pushButtonComInit->setEnabled(true);
			//HaptLinkSupervisor::getInstance()->calibrateHapticA();
		}
		else 
		{
			setStatus("Haptic device A connection failed.  Please check IP is correct and Haptic device is on.  Then recheck the activate box.");
			ui.checkBoxActiveEntactA->setChecked( false );
			ui.checkBoxActiveEntactA->setCheckState( Qt::Unchecked );
			DataLogger::getInstance()->setHapticActiveA( false );
			HaptLinkSupervisor::getInstance()->setHaptActiveA( false );
		}
	}
	else if ( HaptLinkSupervisor::getInstance()->getHaptActiveA() )
		// Must go in stop process
	{
		HaptLinkSupervisor::getInstance()->closeHapticConnectionA();//fix after creating method to close connections of entacts
		setStatus("Haptic device A deactivated.  Press Start to start logging.");
		DataLogger::getInstance()->setHapticActiveA( getEntactAActivate() );
		HaptLinkSupervisor::getInstance()->setHaptActiveA( getEntactAActivate() );
		ui.pushButtonEntactACalibrate->setEnabled( false );
	}
}

void HapticEvaluationGUI::switchEntactB()
{
	if ( getEntactBActivate() )
	{
		if ( HaptLinkSupervisor::getInstance()->initHapticB( 1 , ui.lineEditEntactBIP->text().toLocal8Bit().data() ) == 1 )
		{
			setStatus("Haptic device B activated.  Press Start to start logging.");
			DataLogger::getInstance()->setHapticActiveB( true );
			HaptLinkSupervisor::getInstance()->setHaptActiveB( true );
			ui.pushButtonEntactBCalibrate->setEnabled( true );
			//HaptLinkSupervisor::getInstance()->calibrateHapticB();
		}
		else
		{
			setStatus("Haptic device B connection failed.  Please check IP is correct and Haptic device is on.  Then recheck the activate box.");
			ui.checkBoxActiveEntactB->setChecked( false );
			ui.checkBoxActiveEntactB->setCheckState( Qt::Unchecked );
			DataLogger::getInstance()->setHapticActiveB( false );
			
			HaptLinkSupervisor::getInstance()->setHaptActiveB( false );
		}
	}
	else if ( HaptLinkSupervisor::getInstance()->getHaptActiveB() )
	{
		HaptLinkSupervisor::getInstance()->closeHapticConnectionB();
		setStatus("Haptic device B deactivated.  Press Start to start logging.");
		DataLogger::getInstance()->setHapticActiveB( getEntactBActivate() );
		HaptLinkSupervisor::getInstance()->setHaptActiveB( getEntactBActivate() );
		ui.pushButtonEntactBCalibrate->setEnabled( false );

	}
}





void HapticEvaluationGUI::calibrateEntactA( void )
{
	HaptLinkSupervisor::getInstance()->calibrateHapticA();
}
void HapticEvaluationGUI::calibrateEntactB( void )
{
	HaptLinkSupervisor::getInstance()->calibrateHapticB();
}

void HapticEvaluationGUI::setExpInfo( void )
{
	disableHRFrame(); //disabling haptrep settings
	disableDepthFrame();
	ui.frameHaptRep->setVisible( false );
	ui.frameHaptRepAuto->setVisible( false );
	ui.frameHaptACtrl->setVisible( true );
	ui.frameHaptBCtrl->setVisible( true );
	ui.frameRemCtrl->setVisible( false );
	
	if ( ui.radioButtonForceToNet->isChecked() )
	{			
		ui.frameRemCtrl->setVisible( true );
		ui.frameHaptACtrl->setVisible( false );
		ui.frameHaptBCtrl->setVisible( false );

		//ui.pushButtonStart->setEnabled( false );
		//ui.pushButtonComInit->setEnabled(true);
		
		ui.textEditExpInfo->setPlainText( "Dual Link Force Control through Network:\n"
										  "This control scheme uses a master-master configuration.\n"
										  "-Omni A and B will output a force proportional to the difference in position between them " 
										  "to try and move to the same position.");
	}
	else if ( ui.radioButtonSLPosition->isChecked() )
	{	
		ui.pushButtonStart->setEnabled( true );

		ui.textEditExpInfo->setPlainText( "Single Link Position Control:\n"
										  "This control scheme uses a master-slave configuration.\n"
										  "-Entact A is Master, in force mode outputting no force while sending its position to Entact B.\n"
										  "-Entact B is Slave, reading the position of Entact A and moving to match that position directly." );
	}
	else if ( ui.radioButtonSLForce->isChecked() )
	{
		ui.pushButtonStart->setEnabled( true );
		
		ui.textEditExpInfo->setPlainText( "Single Link Force Control:\n"
										  "This control scheme uses a master-slave configuration.\n"
										  "-Entact A is Master, in force mode outputting no force while sending its position to Entact B.\n"
										  "-Entact B is Slave, reading the position of Entact A and outputting force"
										  " proportional to difference in position between Entact A and B to move itself to Entact A's position" );
	}
	else if ( ui.radioButtonDLForce->isChecked() )
	{
		ui.pushButtonStart->setEnabled( true );

		ui.textEditExpInfo->setPlainText( "Dual Link Force Control:\n"
										  "This control scheme uses a master-master configuration.\n"
										  "-Entact A and B will output a force proportional to the difference in position between them " 
										  "to try and move to the same position.");
	}
	else if ( ui.radioButtonHaptRep->isChecked() || ui.radioButtonHRSeq->isChecked() )
	{
		ui.pushButtonStart->setEnabled( true );

		enableHRFrame();
		ui.frameHaptRep->setVisible( true );
		ui.textEditExpInfo->setPlainText( "Haptic Replication Experiment:\n" 
										  "This experiment tests the ability of a subject to feel a force on one hand and to reproduce it in another.\n" 
										  "-The test subject will feel a force on Entact B, which he/she will need to reproduce.\n"
										  "-The test subject will reproduce the force on Entact A, which will be stationary in force mode." );
	}
	else if ( ui.radioButtonATIOnly->isChecked() )
	{
		ui.pushButtonStart->setEnabled( true );

		ui.textEditExpInfo->setPlainText( "Use ATI Only:\n" 
										  "In this mode, only the ATI sensors are used for measuring force.  That is, the haptic devices are not used." );
	}
	else if ( ui.radioButtonHaptRepAuto->isChecked() )
	{
		ui.frameHaptRepAuto->setVisible( true );

		ui.pushButtonStart->setEnabled( false );
		ui.textEditExpInfo->setPlainText( "Haptic Replication Experiment:\n" 
										  "This experiment tests the ability of a subject to feel a force on one hand and to reproduce it in another.\n" 
										  "-This mode reads from an XML file to automatically run experiments.\n"
										  "-Use the controls in the frame on the right to operate this mode.\n" 
										  "-Valid file must be input and opened before start button is enabled.");
	}
	else if ( ui.radioButtonSingleHaptic->isChecked() )
	{
		ui.pushButtonStart->setEnabled( true );

		enableHRFrame();
		ui.frameHaptRep->setVisible( true );
		ui.textEditExpInfo->setPlainText( "Single Haptic Force Feedback:\n" 
										  "This experiment provides a single force feedback on one device.\n" );
	}
	else if ( ui.radioButtonDepthConstant->isChecked()  )
	{
		ui.pushButtonStart->setEnabled( true );

		DataLogger::getInstance()->setInstruction( true );
			
		enableDepthFrame();
		//ui.lineEditDeltaSlope->setEnabled( false ); // slope is used for Delta

		updateInterfaceGriffith(DEPTHCONST);

		ui.textEditExpInfo->setPlainText( "Single Haptic Force Feedback from z=depth:\n" 
										  "This experiment provides a constant feedback (Force Min + delta Force).\n"
										  "The Delta Slope is used here as a measure, delta, in mm of the distance (z-delta/2; z+delta/2) where the system goes to go to Force Min.\n");

	} 
	else if ( ui.radioButtonDepthLinear->isChecked() )
	{
		ui.pushButtonStart->setEnabled( true );

		DataLogger::getInstance()->setInstruction( true );
			
		enableDepthFrame();
		ui.lineEditDeltaForce->setEnabled( false );
		
		updateInterfaceGriffith(DEPTHLINEAR);
			
		ui.textEditExpInfo->setPlainText( "Single Haptic Force Feedback from z=depth:\n" 
										  "This experiment provides a linear force feedback (ForceMin + linear force= slope*distance.\n");

	}
}

void HapticEvaluationGUI::setHRExpFile( void )
{
	HaptLinkSupervisor::getInstance()->setHaptRepAutoFilename( ui.lineEditExpFile->text() );
	HaptLinkSupervisor::getInstance()->GUINotify( HR_LOAD_FILE );
}
void HapticEvaluationGUI::readNextHRTrial( void )
{
	HaptLinkSupervisor::getInstance()->GUINotify( HR_READ_TRIAL );
}
void HapticEvaluationGUI::setDominantHand( void )
{
	if ( ui.radioButtonLeft->isChecked() )
		HaptLinkSupervisor::getInstance()->setDominance( LEFT );
	else if ( ui.radioButtonRight->isChecked() )
		HaptLinkSupervisor::getInstance()->setDominance( RIGHT );
}
void HapticEvaluationGUI::setGoTo( void )
{
	HaptLinkSupervisor::getInstance()->setGoToIndex( ui.lineEditGoTo->text().toInt() );
	HaptLinkSupervisor::getInstance()->GUINotify( HR_GOTO );
}
outputSide HapticEvaluationGUI::getPref() const
{
	if ( ui.radioButtonLeft->isChecked() )
		return LEFT;
	else if ( ui.radioButtonRight->isChecked() )
		return RIGHT;
	else return RIGHT;
}
void HapticEvaluationGUI::initGraph()
{
	double * buffer = NULL;

	ui.qwtPlot->setTitle("Real Time Graph");
	//ui.qwtPlot->enableAxis(QwtPlot::yRight);
	ui.qwtPlot->setAxisTitle(ui.qwtPlot->xBottom,"Time (s)");
	ui.qwtPlot->setAxisTitle(ui.qwtPlot->yLeft,"Force (N)");
	//ui.qwtPlot->setAxisTitle(ui.qwtPlot->yRight,"Position (m)");
	ui.qwtPlot->setAutoReplot( false );

	zoomer = new QwtPlotZoomer(ui.qwtPlot->canvas());
	zoomer->setTrackerMode(QwtPicker::AlwaysOn);	

	duree         = DURATION;
	period        = PERIOD;
	double perSec = period / 1000.0;
	double freq   = 1 / perSec;
	nbPoints      = int( duree * freq + 1 );
	realIndex     = 0;
	index         = 0;
	dataX         = new double[nbPoints];
	dataFxA       = new double[nbPoints];
	dataFyA       = new double[nbPoints];
	dataFzA       = new double[nbPoints];
    dataFtotalA   = new double[nbPoints];
	dataFxB       = new double[nbPoints];
	dataFyB       = new double[nbPoints];
	dataFzB       = new double[nbPoints];
    dataFtotalB   = new double[nbPoints];
	//dataPx	  = new double[nbPoints];
	timeChannel   = buffer;
	yChannel      = buffer+1;
	double nbPer  = nbPoints - 1;
	dataX[0]      = 0;
	for( int i = 1; i < nbPoints; i++ )
	{
		dataX[i] = dataX[0] + duree * i / nbPer;
		dataFxA[i] = 0.0;
		dataFyA[i] = 0.0;
		dataFzA[i] = 0.0;
		dataFtotalA[i] = 0.0;
		//dataPx[i] = 0.0;
	}
    
	courbeFxA= new QwtPlotCurve( "Fx_A (N)" );
	courbeFxA->setPen(QPen(Qt::yellow));
	courbeFxA->attach(ui.qwtPlot);
	courbeFxA->setRawSamples( dataX, dataFxA, index );

	courbeFxB= new QwtPlotCurve( "Fx_B (N)" );
	courbeFxB->setPen(QPen(Qt::darkYellow));
	courbeFxB->attach(ui.qwtPlot);
		courbeFxB->setRawSamples( dataX, dataFxB, index );
    
	courbeFyA = new QwtPlotCurve( "Fy_A (N)" );
	courbeFyA->setPen(QPen(Qt::green));
	courbeFyA->attach(ui.qwtPlot);
	courbeFyA->setRawSamples( dataX, dataFyA, index );
	
	courbeFyB = new QwtPlotCurve( "Fy_B (N)" );
	courbeFyB->setPen(QPen(Qt::darkGreen));
	courbeFyB->attach(ui.qwtPlot);
	courbeFyB->setRawSamples( dataX, dataFyB, index );
	
	courbeFzA = new QwtPlotCurve( "Fz_A (N)" );
	courbeFzA->setPen(QPen(Qt::blue));
	courbeFzA->attach(ui.qwtPlot);
	courbeFzA->setRawSamples( dataX, dataFzA, index );

	courbeFzB = new QwtPlotCurve( "Fz_B (N)" );
	courbeFzB->setPen(QPen(Qt::darkRed));
	courbeFzB->attach(ui.qwtPlot);
	courbeFzB->setRawSamples( dataX, dataFzB, index );

	courbeFtotalA = new QwtPlotCurve( "F_A (N)" );
	courbeFtotalA->setPen(QPen(Qt::red));
	courbeFtotalA->attach(ui.qwtPlot);
	courbeFtotalA->setRawSamples( dataX, dataFtotalA, index );

	courbeFtotalB = new QwtPlotCurve( "F_B (N)" );
	courbeFtotalB->setPen(QPen(Qt::darkBlue));
	courbeFtotalB->attach(ui.qwtPlot);
	courbeFtotalB->setRawSamples( dataX, dataFtotalB, index );
	
	/*
	courbePx = new QwtPlotCurve("Pos (m)");
	courbePx->setPen(QPen(Qt::cyan));
	courbePx->attach(ui.qwtPlot);
	courbePx->setRawSamples( dataX, dataPx, index );
	courbePx->setYAxis(ui.qwtPlot->yRight);
	*/

	ui.qwtPlot->setAxisScale( ui.qwtPlot->xBottom, dataX[0], dataX[nbPoints-1] );
	ui.qwtPlot->setAxisScale( ui.qwtPlot->yLeft, -12.5, 12.5 );
	//ui.qwtPlot->setAxisScale( ui.qwtPlot->yRight, -0.5, 0.5 );
	
	if( ! KEEP )
	{
		marker = NULL;
	}
	else
	{
		marker = new QwtPlotMarker;
		marker->setValue( QwtDoublePoint( 0, 0 ) );
		marker->setLineStyle(QwtPlotMarker::VLine);
		marker->setLinePen( QPen( Qt::black, 0, Qt::SolidLine ) );
		marker->attach( ui.qwtPlot );
	}
		
    //Grid
    
	Grid= new QwtPlotGrid();
	Grid->enableX(false);
	Grid->setMajorPen(QPen(Qt::darkCyan));
    Grid->attach(ui.qwtPlot); 

//	startTimer( period );
	
	ui.qwtPlot->setAutoReplot( true );

}

void HapticEvaluationGUI::updateDevGraphDisplay(Vector3 g , Vector3 p)
{
	if( index == nbPoints - 1 )
	{
		//dataY[nbPoints-1] = *yChannel;
		dataFxA[nbPoints-1] = g.x;
		dataFyA[nbPoints-1] = g.y;
		dataFzA[nbPoints-1] = g.z;
        dataFtotalA[nbPoints-1] = sqrt((dataFxA[nbPoints-1])*(dataFxA[nbPoints-1])+(dataFyA[nbPoints-1])*(dataFyA[nbPoints-1])+(dataFzA[nbPoints-1])*(dataFzA[nbPoints-1]));
		dataFxB[nbPoints-1] = p.x;
		dataFyB[nbPoints-1] = p.y;
		dataFzB[nbPoints-1] = p.z;
        dataFtotalB[nbPoints-1] = sqrt((dataFxB[nbPoints-1])*(dataFxB[nbPoints-1])+(dataFyB[nbPoints-1])*(dataFyB[nbPoints-1])+(dataFzB[nbPoints-1])*(dataFzB[nbPoints-1]));
        
		//dataPx[nbPoints-1] = p.x;

		dataX[0] = dataX[nbPoints-1];
		for( int i = 1; i < nbPoints; i++ )
		{
			dataX[i] = dataX[0] + duree * i / double( nbPoints - 1 );
		}
		ui.qwtPlot->setAxisScale( QwtPlot::xBottom, dataX[0], dataX[nbPoints-1] );
		index = 0;
	}
	//dataY[index] = *yChannel;
	dataFxA[index] = g.x;
	dataFyA[index] = g.y;
	dataFzA[index] = g.z;
	dataFtotalA[index] = sqrt((dataFxA[index])*(dataFxA[index])+(dataFyA[index])*(dataFyA[index])+(dataFzA[index])*(dataFzA[index]));
	dataFxB[index] = p.x;
	dataFyB[index] = p.y;
	dataFzB[index] = p.z;
	dataFtotalB[index] = sqrt((dataFxB[index])*(dataFxB[index])+(dataFyB[index])*(dataFyB[index])+(dataFzB[index])*(dataFzB[index]));
	
	//dataPx[index] = p.x;
	
	if( marker ) marker->setXValue( double( realIndex ) * period / 1000 );
	index++;
	realIndex++;

	courbeFxA->setRawSamples( dataX, dataFxA, KEEP ? qMin( realIndex, nbPoints ) : index );
	courbeFyA->setRawSamples( dataX, dataFyA, KEEP ? qMin( realIndex, nbPoints ) : index );
	courbeFzA->setRawSamples( dataX, dataFzA, KEEP ? qMin( realIndex, nbPoints ) : index );	
	courbeFtotalA->setRawSamples( dataX, dataFtotalA, KEEP ? qMin( realIndex, nbPoints ) : index );	
	courbeFxB->setRawSamples( dataX, dataFxB, KEEP ? qMin( realIndex, nbPoints ) : index );
	courbeFyB->setRawSamples( dataX, dataFyB, KEEP ? qMin( realIndex, nbPoints ) : index );
	courbeFzB->setRawSamples( dataX, dataFzB, KEEP ? qMin( realIndex, nbPoints ) : index );	
	courbeFtotalB->setRawSamples( dataX, dataFtotalB, KEEP ? qMin( realIndex, nbPoints ) : index );
	//courbePx->setRawSamples( dataX, dataPx, KEEP ? qMin( realIndex, nbPoints ) : index );
}

void HapticEvaluationGUI::disableHRFrame()
{
	if (! ui.radioButtonSingleHaptic->isChecked() ) { 
		ui.lineEditHRFx->setEnabled( false ); //disabling hapt replication settings
		ui.lineEditHRFy->setEnabled( false );
		ui.lineEditHRFz->setEnabled( false );
		ui.lineEditHRTx->setEnabled( false );
		ui.lineEditHRTy->setEnabled( false );
		ui.lineEditHRTz->setEnabled( false );
		ui.radioButtonSideDom->setEnabled( false );
		ui.radioButtonSideNonDom->setEnabled( false );
	}
}
void HapticEvaluationGUI::enableHRFrame()
{
	ui.lineEditHRFx->setEnabled( true );
	ui.lineEditHRFy->setEnabled( true );
	ui.lineEditHRFz->setEnabled( true );
	ui.lineEditHRTx->setEnabled( true );
	ui.lineEditHRTy->setEnabled( true );
	ui.lineEditHRTz->setEnabled( true );
	ui.radioButtonSideDom->setEnabled( true );
	ui.radioButtonSideNonDom->setEnabled( true );
}
void HapticEvaluationGUI::disableDepthFrame() 
{
	if (! (ui.radioButtonDepthLinear ->isChecked()||ui.radioButtonDepthConstant->isChecked()  ) ) { 
	ui.lineEditDeltaForce->setEnabled( false );
	ui.lineEditDeltaSlope->setEnabled( false );
	ui.lineEditDistance->setEnabled( false );
	ui.lineEditForceMin->setEnabled( false );
	}
}
void HapticEvaluationGUI::enableDepthFrame()
{
	
	ui.lineEditDeltaForce->setEnabled( true );
	ui.lineEditDeltaSlope->setEnabled( true );
	ui.lineEditDistance->setEnabled( true );
	ui.lineEditForceMin->setEnabled( true );
}

void HapticEvaluationGUI::showDataA()
{
	ui.labelFx_A->setVisible( true );
	ui.labelFy_A->setVisible( true );
	ui.labelFz_A->setVisible( true );
	ui.labelTx_A->setVisible( true );
	ui.labelTy_A->setVisible( true );
	ui.labelTz_A->setVisible( true );
	ui.checkBoxFxA->setEnabled( true );
	ui.checkBoxFyA->setEnabled( true );
	ui.checkBoxFzA->setEnabled( true );
	ui.checkBoxFtotalA->setEnabled( true );
}
void HapticEvaluationGUI::hideDataA()
{
	ui.labelFx_A->setVisible( false );
	ui.labelFy_A->setVisible( false );
	ui.labelFz_A->setVisible( false );
	ui.labelTx_A->setVisible( false );
	ui.labelTy_A->setVisible( false );
	ui.labelTz_A->setVisible( false );
	ui.checkBoxFxA->setEnabled( false );
	ui.checkBoxFyA->setEnabled( false );
	ui.checkBoxFzA->setEnabled( false );
	ui.checkBoxFtotalA->setEnabled( false );
}
void HapticEvaluationGUI::showDataB()
{
	ui.labelFx_B->setVisible( true );
	ui.labelFy_B->setVisible( true );
	ui.labelFz_B->setVisible( true );
	ui.labelTx_B->setVisible( true );
	ui.labelTy_B->setVisible( true );
	ui.labelTz_B->setVisible( true );
	ui.checkBoxFxB->setEnabled( true );
	ui.checkBoxFyB->setEnabled( true );
	ui.checkBoxFzB->setEnabled( true );
	ui.checkBoxFtotalB->setEnabled( true );
}
void HapticEvaluationGUI::hideDataB()
{
	ui.labelFx_B->setVisible( false );
	ui.labelFy_B->setVisible( false );
	ui.labelFz_B->setVisible( false );
	ui.labelTx_B->setVisible( false );
	ui.labelTy_B->setVisible( false );
	ui.labelTz_B->setVisible( false );
	ui.checkBoxFxB->setEnabled( false );
	ui.checkBoxFyB->setEnabled( false );
	ui.checkBoxFzB->setEnabled( false );
	ui.checkBoxFtotalB->setEnabled( false );
}

void HapticEvaluationGUI::showDataA_2()
{
	ui.labelFx_A_2->setVisible( true );
	ui.labelFy_A_2->setVisible( true );
	ui.labelFz_A_2->setVisible( true );
	ui.labelTx_A_2->setVisible( true );
	ui.labelTy_A_2->setVisible( true );
	ui.labelTz_A_2->setVisible( true );
	ui.checkBoxFzA_2->setEnabled( true );
}
void HapticEvaluationGUI::hideDataA_2()
{
	ui.labelFx_A_2->setVisible( false );
	ui.labelFy_A_2->setVisible( false );
	ui.labelFz_A_2->setVisible( false );
	ui.labelTx_A_2->setVisible( false );
	ui.labelTy_A_2->setVisible( false );
	ui.labelTz_A_2->setVisible( false );
	ui.checkBoxFzA_2->setEnabled( false );
}
void HapticEvaluationGUI::showDataB_2()
{
	ui.labelFx_B_2->setVisible( true );
	ui.labelFy_B_2->setVisible( true );
	ui.labelFz_B_2->setVisible( true );
	ui.labelTx_B_2->setVisible( true );
	ui.labelTy_B_2->setVisible( true );
	ui.labelTz_B_2->setVisible( true );
	ui.checkBoxFxB_2->setEnabled( true );
}
void HapticEvaluationGUI::hideDataB_2()
{
	ui.labelFx_B_2->setVisible( false );
	ui.labelFy_B_2->setVisible( false );
	ui.labelFz_B_2->setVisible( false );
	ui.labelTx_B_2->setVisible( false );
	ui.labelTy_B_2->setVisible( false );
	ui.labelTz_B_2->setVisible( false );
	ui.checkBoxFxB_2->setEnabled( false );
}
void HapticEvaluationGUI::disableInterface()
{
	
	ui.lineEditLJSerialA->setEnabled( false );
	ui.lineEditLJSerialB->setEnabled( false );
	ui.checkBoxAtiActivateA->setEnabled( false );
	ui.checkBoxAtiActivateB->setEnabled( false );
	ui.lineEditLog->setEnabled( false );
	//ui.lineEditAge->setEnabled( false );
	//ui.radioButtonMale->setEnabled( false );
	//ui.radioButtonFemale->setEnabled( false );
	ui.radioButtonRight->setEnabled( false );
	ui.radioButtonLeft->setEnabled( false );
	ui.lineEditEntactAIP->setEnabled( false );
	ui.lineEditEntactBIP->setEnabled( false );
	ui.checkBoxActiveEntactA->setEnabled( false );
	ui.checkBoxActiveEntactB->setEnabled( false );
	ui.pushButtonEntactACalibrate->setEnabled( false );
	ui.pushButtonEntactBCalibrate->setEnabled( false );
	ui.radioButtonSLPosition->setEnabled( false );
	
	//******
	ui.radioButtonForceToNet->setEnabled( false );
	
	ui.radioButtonSLForce->setEnabled( false );
	ui.radioButtonDLForce->setEnabled( false );
	ui.radioButtonHaptRep->setEnabled( false );
	ui.radioButtonATIOnly->setEnabled( false );
	ui.radioButtonSingleHaptic->setEnabled( false );
	ui.radioButtonHRSeq->setEnabled( false );
	ui.radioButtonHaptRepAuto->setEnabled( false );
}
/*
// TODO: On essaye de faire sans.
void HapticEvaluationGUI::enableInterface()
{
	
	ui.lineEditLJSerialA->setEnabled( true );
	ui.lineEditLJSerialB->setEnabled( true );
	ui.checkBoxAtiActivateA->setEnabled( true );
	ui.checkBoxAtiActivateB->setEnabled( true );
	ui.lineEditLog->setEnabled( true );
	//ui.lineEditAge->setEnabled( true );
	//ui.radioButtonMale->setEnabled( true );
	//ui.radioButtonFemale->setEnabled( true );
	ui.radioButtonRight->setEnabled( true );
	ui.radioButtonLeft->setEnabled( true );
	ui.lineEditEntactAIP->setEnabled( true );
	ui.lineEditEntactBIP->setEnabled( true );
	ui.checkBoxActiveEntactA->setEnabled( true );
	ui.checkBoxActiveEntactB->setEnabled( true );
	ui.pushButtonEntactACalibrate->setEnabled( true );
	ui.pushButtonEntactBCalibrate->setEnabled( true );
	ui.radioButtonSLPosition->setEnabled( true );

	//******
	ui.radioButtonForceToNet->setEnabled( true );

	ui.radioButtonSLForce->setEnabled( true );
	ui.radioButtonDLForce->setEnabled( true );
	ui.radioButtonHaptRep->setEnabled( true );
	ui.radioButtonATIOnly->setEnabled( true );
	ui.radioButtonSingleHaptic->setEnabled( true );
	ui.radioButtonHRSeq->setEnabled( true );
	ui.radioButtonHaptRepAuto->setEnabled( true );
}


void HapticEvaluationGUI::setHRParameterDisplay()
{
	HRExperiment experiment = HaptLinkSupervisor::getInstance()->getExperiment();
	ui.labelExpIndex->setText( QString::fromStdString(experiment.toString( experiment.getIndex())) );
	ui.labelExpMagnitude->setText( QString::fromStdString(experiment.toString( experiment.getMagnitude())) );
	ui.labelExpDirection->setText( QString::fromStdString(experiment.toString( experiment.getDirection())) );
	ui.labelExpSign->setText( QString::fromStdString(experiment.toString( experiment.getSign())) );
	ui.labelExpOutputHand->setText( QString::fromStdString(experiment.toString( experiment.getHand())) );
	ui.labelExpDistance->setText( QString::fromStdString(experiment.toString( experiment.getDistance())) );
	ui.labelExpDelta->setText( QString::fromStdString(experiment.toString( experiment.getDeltaDepth())) );
	ui.labelExpForceMin->setText( QString::fromStdString(experiment.toString( experiment.getForceMin())) );

	ui.labelIsRun->setText( "No" );
}
*/

void HapticEvaluationGUI::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_A) 
	{
		setStatus("Key A Pressed");

		int militime = HaptLinkSupervisor::getInstance()->getTimeStamp();	
		DataLogger::getInstance()->setAkey(militime);
		/*
		DataLogger::getInstance()->xml.writeStartElement("KeyA" );
		DataLogger::getInstance()->xml.writeTextElement("TimeStamp_ms",QString::number(militime));		
		//DataLogger::getInstance()->WritePacket(HaptLinkSupervisor::getInstance()->getPosition(),"Position_m");
		DataLogger::getInstance()->xml.writeEndElement();
		*/
    } 
	if (event->key() == Qt::Key_Z) 
	{
		setStatus("Key Z Pressed");
				
		int militime = HaptLinkSupervisor::getInstance()->getTimeStamp();	
		DataLogger::getInstance()->setZkey(militime);
		/*
		HaptLinkSupervisor::getInstance()->getMutex()->lock(); // mutex
		DataLogger::getInstance()->xml.writeStartElement( "KeyZ" );
		DataLogger::getInstance()->xml.writeTextElement( "Finished",QString::number(militime) );		
		//DataLogger::getInstance()->WritePacket(HaptLinkSupervisor::getInstance()->getPosition(),"Position_m");
		DataLogger::getInstance()->xml.writeEndElement();
		HaptLinkSupervisor::getInstance()->getMutex()->unlock(); // end mutex
		*/
    } 
	if (event->key() == Qt::Key_C) 
	{
		

		if ( ui.radioButtonHRSeq->isChecked() )
		{
			setStatus("Key C Pressed");
			HaptLinkSupervisor::getInstance()->GUINotify( HR_SEQ_SWITCH );
		}
    } 
}

void HapticEvaluationGUI::updateInterfaceGriffith(expType type) {

 if (type == DEPTHCONST)
	 ui.label_41->setText("delta depth (mm)");

 if (type == DEPTHLINEAR)
	 ui.label_41->setText("delta Slope (>0)");

  ui.label_41->show();

  ui.label->setText("added FX_B");
  ui.label->show();
  ui.label_2->setText("added FY_B");
  ui.label_2->show();
  ui.label_3->setText("added FZ_B");
  ui.label_3->show();

  ui.label_8->setText("X_B");
  ui.label_8->show();
  ui.label_9->setText("Y_B");
  ui.label_9->show();
  ui.label_7->setText("Z_B");
  ui.label_7->show();
   
}

void HapticEvaluationGUI::updateGriffith(Vector3 t1, Vector3 f1)
{
	QString Tx_A = QString::number(t1.x,'f',3);
	ui.labelTx_A->setText(Tx_A);
	ui.labelTx_A->show();

	QString Ty_A = QString::number(t1.y,'f',3);
	ui.labelTy_A->setText(Ty_A);
	ui.labelTy_A->show();

	QString Tz_A = QString::number(t1.z,'f',3);
	ui.labelTz_A->setText(Tz_A);
	ui.labelTz_A->show();

	
	QString Fx_A = QString::number(f1.x,'f',3);
	ui.labelFx_A->setText(Fx_A);
	ui.labelFx_A->show();

	QString Fy_A = QString::number(f1.y,'f',3);
	ui.labelFy_A->setText(Fy_A);
	ui.labelFy_A->show();

	QString Fz_A = QString::number(f1.z,'f',3);	
	ui.labelFz_A->setText(Fz_A);
	ui.labelFz_A->show();
}

void HapticEvaluationGUI::switchDisplay()
{
	if (ui.frame_5->isVisible())
	{
		ui.frame_5->setVisible(false);
		ui.frame_6->setVisible(true);
	}
	else if(ui.frame_6->isVisible())
	{
		ui.frame_5->setVisible(true);
		ui.frame_6->setVisible(false);
	}
}



