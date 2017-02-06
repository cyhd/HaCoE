#include "bottomwidget.h"


BottomWidget::BottomWidget(NewWindow *parent) : QWidget(parent)
{
	newWindow = parent;

	pbGraphForce = new QPushButton("Graphe Force");
	pbStop = new QPushButton("Stop");
	pbStart = new QPushButton("Start");
	pbZero = new QPushButton("Zero");
	pbConnect = new QPushButton("Connect");
	pbHelp = new QPushButton("Help");
	lay = new QHBoxLayout(this);
	lay->addWidget(pbHelp);
	lay->addWidget(pbGraphForce);
	lay->addWidget(pbConnect);
	lay->addWidget(pbStart);
	lay->addWidget(pbZero);
	lay->addWidget(pbStop);
	setLayout(lay);

	// CONNECT BUTTONS
	
	QObject::connect(pbStart , SIGNAL(clicked()) , this , SLOT(startProcess()));
	QObject::connect(pbStop , SIGNAL(clicked()) , this , SLOT(stopProcess()));
	QObject::connect(pbConnect , SIGNAL(clicked()) , this , SLOT(initCommunication()));
}


BottomWidget::~BottomWidget(void)
{
}

void BottomWidget::initCommunication(){
	std::cout<<"BottomWidget::initCommunication"<<std::endl;
	ExperienceWidget* currentExp = qobject_cast<ExperienceWidget*>(newWindow->centerWidget->currentWidget());
	currentExp->applySettings();
	activateCorrectDevice();
	HaptLinkSupervisor::getInstance()->start();
	
	int localPort;
	string remoteIP;
	string remotePort;
	int delay;

	if (newWindow->topWidget->rbUser1->isChecked()){
		localPort = newWindow->topWidget->tePort1->text().toUInt();
		remotePort = newWindow->topWidget->tePort2->text().toStdString().data();
		remoteIP = newWindow->topWidget->teIP2->text().toStdString().data();
		delay = newWindow->topWidget->teDelay->text().toInt();
	}
	else if (newWindow->topWidget->rbUser2->isChecked()){
		localPort = newWindow->topWidget->tePort2->text().toUInt();
		remotePort = newWindow->topWidget->tePort1->text().toStdString().data();
		remoteIP = newWindow->topWidget->teIP1->text().toStdString().data();
		delay = newWindow->topWidget->teDelay->text().toInt();
	}
	else
	{
		// TODO: message to tell the user to fill in the fields (or desactive)
		return;
	}
	std::cout<<"BottomWidget::initCommunication:SETTINGS ----"<<std::endl;
	std::cout<<"BottomWidget::initCommunication:localPort:"<<localPort<<std::endl;
	std::cout<<"BottomWidget::initCommunication:remotePort:"<<remotePort<<std::endl;
	std::cout<<"BottomWidget::initCommunication:remoteIP:"<<remoteIP<<std::endl;
	std::cout<<"BottomWidget::initCommunication:delay:"<<delay<<std::endl;

	std::cout<<"BottomWidget::initCommunication:initUDPReadWrite"<<std::endl;

	// TODO: BUg HERE
	HaptLinkSupervisor::getInstance()->initUDPReadWrite(localPort, remoteIP, remotePort, delay);
}


void BottomWidget::startProcess(){
	std::cout<<"BottomWidget::startProcess"<<std::endl;
	
	// You need to run the activate commands before the initCommunication: Search for switchAtiA
	
	// Set settings for selected experiment
	

	startLogging();

	// UI
	this->setStyleSheet("background-color: green;");
	repaint();
	pbZero->setEnabled( true );
	pbStop->setEnabled( true );
	pbStart->setEnabled( false );
}

void BottomWidget::startLogging()
{
	std::cout<<"BottomWidget::startLogging"<<std::endl;
	filename.clear(); // clear away old text before appending filename
	date = dateTime->currentDateTime().toString( "yyyyMMdd_hhmmss" );
	filename.append( date + newWindow->topWidget->teLogFile->text() + ".xml");
	
	// TODO
	// setStatus("Logging.");
	
	DataLogger::getInstance()->OpenSessionLog(filename);
}

void BottomWidget::stopProcess(){
	// TODO: Check order here
	HaptLinkSupervisor::getInstance()->stop();
	deactivateCorrectDevice();

	stopLogging();

	// UI
	this->setStyleSheet("background-color: red;");
	repaint();
	pbZero->setEnabled( false );
	pbStop->setEnabled( false );
	pbStart->setEnabled( true );
}

void BottomWidget::stopLogging()
{
	DataLogger::getInstance()->CloseSessionLog();
	//setStatus("Logging Complete");
}



/* ON A CHOISI DE LAISSER CA LA PARCE QUON SAIT PAS A QUOI CA SERT
C'est cense etre dans le start process et dans les applysettings des experiences correspondantes

	// dual haptic device check
	if (ui.radioButtonSLPosition->isChecked() || ui.radioButtonSLForce->isChecked() || ui.radioButtonDLForce->isChecked() || ui.radioButtonHaptRep->isChecked() || ui.radioButtonHaptRepAuto->isChecked())
	{ 
		if (!( getEntactAActivate() && getEntactBActivate() )) 
		{
			setStatus( "Experiment selected but Entact devices not enabled.  Please enable them and try again." );
			return;
		}
	}
	// single haptic device check	
	if ( ui.radioButtonSingleHaptic->isChecked() ||  ui.radioButtonDepthConstant->isChecked() || ui.radioButtonDepthLinear->isChecked() ) 
	{
		if ( !( getEntactAActivate() || getEntactBActivate() ))
		{
			setStatus( "Experiment selected but Entact devices not enabled.  Please enable one device on the dominant hand and try again." );
			return;
		}
	}
	*/


void BottomWidget::activateCorrectDevice() // Old switchEntactA
{
	std::cout<<"BottomWidget::activateCorrectDevice"<<std::endl;
	
	QString IP;
	int choice;
	// Get info fro, selected user
	if (newWindow->topWidget->rbUser1->isChecked()){
		std::cout<<"BottomWidget::activateCorrectDevice:selected user:1"<<std::endl;
		IP = newWindow->topWidget->teIP1->text();
		choice = 0;
	}
	else {
		std::cout<<"BottomWidget::activateCorrectDevice:selected user:2"<<std::endl;
		IP = newWindow->topWidget->teIP2->text();
		choice = 1;
	}

	int connexion_result = HaptLinkSupervisor::getInstance()->initHapticA(choice, IP.toLocal8Bit().data() ) ; 
	
	if ( connexion_result == 1 ) // connexion success
	{
		std::cout<<"BottomWidget::activateCorrectDevice:connexion success"<<std::endl;
		//setStatus("Haptic device A activated.  Press Start to start logging.");
		DataLogger::getInstance()->setHapticActiveA( true );
		HaptLinkSupervisor::getInstance()->setHaptActiveA( true );
		// ui.pushButtonEntactACalibrate->setEnabled( true ); TODO
	}
	else // connexion fail
	{
		std::cout<<"BottomWidget::activateCorrectDevice:connexion fail"<<std::endl;
		//setStatus("Haptic device A connection failed.  Please check IP is correct and Haptic device is on.  Then recheck the activate box.");
		DataLogger::getInstance()->setHapticActiveA( false );
		HaptLinkSupervisor::getInstance()->setHaptActiveA( false );
	}
}

void BottomWidget::deactivateCorrectDevice()
{
	std::cout<<" BottomWidget::deactivateCorrectDevice"<<std::endl;
	HaptLinkSupervisor::getInstance()->closeHapticConnectionA(); //fix after creating method to close connections of entacts
	// setStatus("Haptic device A deactivated.  Press Start to start logging.");
	DataLogger::getInstance()->setHapticActiveA(false);
	HaptLinkSupervisor::getInstance()->setHaptActiveA(false);
	// TODO ui.pushButtonEntactACalibrate->setEnabled( false );
}


void BottomWidget::enableButtons()
{
	std::cout<<" BottomWidget::enableButtons"<<std::endl;
	pbGraphForce->setEnabled(true);
	pbStop->setEnabled(true);
	pbStart->setEnabled(true);
	pbZero->setEnabled(true);
	pbConnect->setEnabled(true);
}

void BottomWidget::disableButtons()
{
	std::cout<<"BottomWidget::disableButtons"<<std::endl;
	pbGraphForce->setEnabled(false);
	pbStop->setEnabled(false);
	pbStart->setEnabled(false);
	pbZero->setEnabled(false);
	pbConnect->setEnabled(false);
}