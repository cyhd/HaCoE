#include "bottomwidget.h"


BottomWidget::BottomWidget(NewWindow *parent) : QWidget(parent)
{
	newWindow = parent;

	lLog = new QLabel("Log Info");
	teLog = new QTextEdit();
	teLog->setReadOnly(true);
	pbGraphForce = new QPushButton("Graphe Force");
	pbStop = new QPushButton("Stop");
	pbStart = new QPushButton("Start");
	pbCalibrate = new QPushButton("Calibrate");
	// ACTIVATE STACK
	pbActivate = new QPushButton("Activate");

	// CONNECT STACK
	pbConnect = new QPushButton("Connect");

	pbZero = new QPushButton("Zero");
	pbHelp = new QPushButton("Help");

	lay = new QGridLayout(this);
	lay->addWidget(lLog,0,0);
	lay->addWidget(teLog,1,0,1,8);
	lay->addWidget(pbHelp,2,0);
	lay->addWidget(pbGraphForce,2,1);
	lay->addWidget(pbActivate,2,2);
	lay->addWidget(pbConnect,2,3);
	lay->addWidget(pbCalibrate,2,4);
	lay->addWidget(pbStart,2,5);
	lay->addWidget(pbZero,2,6);
	lay->addWidget(pbStop,2,7);
	setLayout(lay);

	// CONNECT BUTTONS
	connectPbActivate();
	connectPbConnect();
	connectPbStart();
	connectPbStop();
	connectPbCalibrate();
	QObject::connect(pbHelp, SIGNAL(clicked()), this, SLOT(showHelpDialog()));
}


BottomWidget::~BottomWidget(void)
{
}


void BottomWidget::activateProcess(){

	std::cout<<"BottomWidget::activateProcess"<<std::endl;
 	ExperienceWidget* currentExp = qobject_cast<ExperienceWidget*>(newWindow->centerWidget->currentWidget());
	currentExp->applySettings();
	activateCorrectDevice();

	setStatus("Activate Process");
}

void BottomWidget::connectProcess(){
	std::cout<<"BottomWidget::initCommunication"<<std::endl;

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

	HaptLinkSupervisor::getInstance()->start();
	HaptLinkSupervisor::getInstance()->initUDPReadWrite(localPort, remoteIP, remotePort, delay);

	setStatus("Connection...");
}


void BottomWidget::startProcess(){
	std::cout<<"BottomWidget::startProcess"<<std::endl;
	// HaptLinkSupervisor::getInstance()->start(); moved in init communication

	startLogging();

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
	
	setStatus("Logging");
	
	DataLogger::getInstance()->OpenSessionLog(filename);
}

void BottomWidget::stopProcess(){
	// TODO: Check order here
	HaptLinkSupervisor::getInstance()->stop();
	//deactivateCorrectDevice();

//	stopLogging();
	setStatus("Stop Process.");
}

void BottomWidget::stopLogging()
{
	DataLogger::getInstance()->CloseSessionLog();
	setStatus("Logging Complete");
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
	// Get info from selected user
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
		setStatus("Haptic device A activated.  Press Start to start logging.");
		DataLogger::getInstance()->setHapticActiveA( true );
		HaptLinkSupervisor::getInstance()->setHaptActiveA( true );
		// ui.pushButtonEntactACalibrate->setEnabled( true ); TODO
	}
	else // connexion fail
	{
		std::cout<<"BottomWidget::activateCorrectDevice:connexion fail"<<std::endl;
		setStatus("Haptic device A connection failed.  Please check IP is correct and Haptic device is on.  Then recheck the activate box.");
		DataLogger::getInstance()->setHapticActiveA( false );
		HaptLinkSupervisor::getInstance()->setHaptActiveA( false );
	}

	setStatus("Activate device");
}

void BottomWidget::deactivateCorrectDevice()
{
	std::cout<<"BottomWidget::deactivateCorrectDevice"<<std::endl;
	HaptLinkSupervisor::getInstance()->closeHapticConnectionA(); //fix after creating method to close connections of entacts
	setStatus("Haptic device A deactivated.  Press Start to start logging.");
	DataLogger::getInstance()->setHapticActiveA(false);
	HaptLinkSupervisor::getInstance()->setHaptActiveA(false);
	// TODO ui.pushButtonEntactACalibrate->setEnabled( false );

	setStatus("Desactivate Device");
}


void BottomWidget::calibrateCorrectDevice()
{
	std::cout<<"BottomWidget::calibrateCorrectDevice"<<std::endl;
	HaptLinkSupervisor::getInstance()->calibrateCorrectDevice();

	setStatus("Calibrate Device");
}

void BottomWidget::setStatus(string s){ 
	teLog->moveCursor (QTextCursor::End);
	teLog->insertPlainText(QString::fromStdString(s));
	teLog->insertPlainText ("\n");
}


void BottomWidget::disableButtons()
{
	std::cout<<"BottomWidget::disableButtons"<<std::endl;
	pbGraphForce->setEnabled(false);

	disableActivate();
	disableConnect();
	disableStop();
	disableStart();
	disableCalibrate();
	disableZero();
}


void BottomWidget::enableActivate()
{
	std::cout<<"BottomWidget::enableActivate"<<std::endl;
	pbActivate->setEnabled(true);
}


void BottomWidget::enableConnect()
{
	std::cout<<"BottomWidget::enableConnect"<<std::endl;
	pbConnect->setEnabled(true);
}

void BottomWidget::enableCalibrate()
{
	std::cout<<"BottomWidget::enableCalibrate"<<std::endl;
	pbCalibrate->setEnabled(true);
}

void BottomWidget::enableStart()
{
	std::cout<<"BottomWidget::enableStart"<<std::endl;
	pbStart->setEnabled(true);
}


void BottomWidget::enableStop()
{
	std::cout<<"BottomWidget::enableStop"<<std::endl;
	pbStop->setEnabled(true);
}


void BottomWidget::disableActivate()
{
	std::cout<<"BottomWidget::disableActivate"<<std::endl;
	pbActivate->setEnabled(false);
}


void BottomWidget::disableConnect()
{
	std::cout<<"BottomWidget::disableConnect"<<std::endl;
	pbConnect->setEnabled(false);
}


void BottomWidget::disableStart()
{
	std::cout<<"BottomWidget::disableStart"<<std::endl;
	pbStart->setEnabled(false);
}

void BottomWidget::disableZero()
{
	std::cout<<"BottomWidget::disableZero"<<std::endl;
	pbZero->setEnabled(false);
}


void BottomWidget::disableCalibrate()
{
	std::cout<<"BottomWidget::disableCalibrate"<<std::endl;
	pbCalibrate->setEnabled(false);
}

void BottomWidget::disableStop()
{
	std::cout<<"BottomWidget::disableStop"<<std::endl;
	pbStop->setEnabled(false);
}


// CONNECTS


void BottomWidget::connectPbActivate(){
	QObject::connect(pbActivate , SIGNAL(clicked()) , this , SLOT(activateProcess()));
	QObject::connect(pbActivate , SIGNAL(clicked()) , this , SLOT(enableConnect()));
	QObject::connect(pbActivate , SIGNAL(clicked()) , this , SLOT(disableActivate()));
	QObject::connect(pbActivate , SIGNAL(clicked()) , this , SLOT(enableStop()));
	QObject::connect(pbActivate , SIGNAL(clicked()) , this , SLOT(enableCalibrate()));
}


void BottomWidget::connectPbConnect(){
	QObject::connect(pbConnect , SIGNAL(clicked()) , this , SLOT(connectProcess()));
	QObject::connect(pbConnect , SIGNAL(clicked()) , this , SLOT(enableStart()));
	QObject::connect(pbConnect , SIGNAL(clicked()) , this , SLOT(enableStop()));
	QObject::connect(pbConnect , SIGNAL(clicked()) , this , SLOT(disableConnect()));
	QObject::connect(pbConnect , SIGNAL(clicked()) , this , SLOT(disableCalibrate()));
}

void BottomWidget::connectPbStart(){
	QObject::connect(pbStart , SIGNAL(clicked()) , this , SLOT(startProcess()));
	QObject::connect(pbStart , SIGNAL(clicked()) , this , SLOT(enableStop()));
	QObject::connect(pbStart , SIGNAL(clicked()) , this , SLOT(disableStart()));
	QObject::connect(pbStart , SIGNAL(clicked()) , this , SLOT(disableConnect()));
}
void BottomWidget::connectPbCalibrate(){
	QObject::connect(pbCalibrate , SIGNAL(clicked()) , this , SLOT(calibrateCorrectDevice()));
}

void BottomWidget::connectPbStop(){
	QObject::connect(pbStop , SIGNAL(clicked()) , this , SLOT(stopProcess()));
	QObject::connect(pbStop , SIGNAL(clicked()) , this , SLOT(disableButtons()));
	QObject::connect(pbStop , SIGNAL(clicked()) , this , SLOT(enableActivate()));
	QObject::connect(pbStop , SIGNAL(clicked()) , this , SLOT(disableStop()));
}

// QDialog
void BottomWidget::showHelpDialog(){
	std::cout<<"BottomWidget::showHelpDialog"<<std::endl;
	QDialog *definition = new QDialog (this);
	definition->setFixedSize(500,300);
	QLabel *label_definition = new QLabel (" 1- Choose your experience\n 2- Select your robot\n 3- Select your size for the experiment (for example for force to network, if you chose James, \nput your IP and your port in IP James and Port James in Configuration, the other user \ndo the same for him and select Bond)\n 4- Make the configuration\n\n\n Once the configuration is done: \n\n Activate -> Connect -> Start -> Stop \n\n\nIf \"Activate\" button does not work, check your Omni robot name is set to \"Phantom1\"", definition);
	definition->exec();										  
}
