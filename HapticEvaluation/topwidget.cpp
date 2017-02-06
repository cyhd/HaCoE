#include "topwidget.h"


TopWidget::TopWidget(NewWindow *parent) : QWidget(parent)
{
	newWindow = parent;
	initUI();
}

void TopWidget::initUI()
{
	// Experience
	gbExperience = new QGroupBox("1- Choose Experience");
	QGridLayout *layExperience = new QGridLayout();
	lbExperience = new QLabel("Experience: ");
	cbExperience = new QComboBox();
	cbExperience->addItems(QStringList() 
		<< "NONE" 
		<< "ATI Only" 
		<< "Single Link Position Control"
		<< "Single Link Force Control" 
		<< "Dual Link Force Control" 
		<< "Haptic Replication Simultaneous" 
		<< "Haptic Replication Sequential" 
		<< "Haptic Replication Auto" 
		<< "Single force Feedback" 
		<< "Force with depth constar" 
		<< "Force with Depth linear"
		<< "Force to network" );
	layExperience->addWidget(lbExperience, 0, 0);
	layExperience->addWidget(cbExperience, 0, 1);
	gbExperience->setLayout(layExperience);


	// Robot type
	gbRobot = new QGroupBox("2- Robot: ");
	rbRobot1 = new QRadioButton("Omni");
	rbRobot2 = new QRadioButton("Phantom X alpha 12");

	QVBoxLayout *layRobot = new QVBoxLayout();
    layRobot->addWidget(rbRobot1);
    layRobot->addWidget(rbRobot2);
    gbRobot->setLayout(layRobot);

	// Network Type
	gbNetworkType = new QGroupBox("3- Connexion type: ");
	rbNetworkType1 = new QRadioButton("Local");
	rbNetworkType2 = new QRadioButton("Network");

	QVBoxLayout *layNetworkType = new QVBoxLayout();
    layNetworkType->addWidget(rbNetworkType1);
    layNetworkType->addWidget(rbNetworkType2);

    gbNetworkType->setLayout(layNetworkType);

	// User choice
	gbUserChoice = new QGroupBox("4- Who am I ?");
	rbUser1 = new QRadioButton("James");
	rbUser2 = new QRadioButton("Bond");

	QVBoxLayout *layUserChoice = new QVBoxLayout();
    layUserChoice->addWidget(rbUser1);
    layUserChoice->addWidget(rbUser2);
    gbUserChoice->setLayout(layUserChoice);

	// IP and Port and delay
	gbConfig = new QGroupBox("5- Configuration");
	lbIP1 = new QLabel("IP James :");
	teIP1 = new QLineEdit();
	lbPort1 = new QLabel("Port James :");
	tePort1 = new QLineEdit();
	lbIP2 = new QLabel("IP Bond :");
	teIP2 = new QLineEdit();
	lbPort2 = new QLabel("Port Bond :");
	tePort2 = new QLineEdit();
	// Delay text field
	lbDelay = new QLabel("Delay:");
	teDelay = new QLineEdit();

	QGridLayout *layConfigIP = new QGridLayout();
	layConfigIP->addWidget(lbIP1, 0, 0);
	layConfigIP->addWidget(teIP1, 0, 1);
	layConfigIP->addWidget(lbPort1, 0, 2);
	layConfigIP->addWidget(tePort1, 0, 3);
	layConfigIP->addWidget(lbIP2, 1, 0);
	layConfigIP->addWidget(teIP2, 1, 1);
	layConfigIP->addWidget(lbPort2, 1, 2);
	layConfigIP->addWidget(tePort2, 1, 3);
	layConfigIP->addWidget(lbDelay, 1, 4); // Redefine this position
	layConfigIP->addWidget(teDelay, 1, 5);
	gbConfig->setLayout(layConfigIP);

	// LogFile
	gbLogFile = new QGroupBox("6- Optional");
	QGridLayout *layLogFile = new QGridLayout();
	lbLogFile = new QLabel("Log File : ");
	teLogFile = new QLineEdit();
	layLogFile->addWidget(lbLogFile,0 ,0);
	layLogFile->addWidget(teLogFile,0 ,1);
	gbLogFile->setLayout(layLogFile);

	// INIT PHASE
	resetUI();

	//Add Layout stuff
	lay = new QGridLayout(this);
	lay->addWidget(gbExperience, 0, 0,  1, 3);
	
	lay->addWidget(gbRobot, 0, 3);
	lay->addWidget(gbNetworkType, 1, 0);
	lay->addWidget(gbUserChoice, 1, 2);
	lay->addWidget(gbConfig, 1, 3);
	lay->addWidget(gbLogFile, 2, 0, 1, 4);

	setLayout(lay);


	// CONNECTS
	QObject::connect(cbExperience, SIGNAL(currentIndexChanged(int)), this, SLOT(experienceChosen(int)));
	QObject::connect(rbRobot1, SIGNAL(clicked()), this, SLOT(enableGbNetworkType()));
	QObject::connect(rbRobot2, SIGNAL(clicked()), this, SLOT(enableGbNetworkType()));

	QObject::connect(rbNetworkType1, SIGNAL(clicked()), this, SLOT(enableGbUserChoice()));
	QObject::connect(rbNetworkType2, SIGNAL(clicked()), this, SLOT(enableGbUserChoice()));

	QObject::connect(rbUser1, SIGNAL(clicked()), this, SLOT(enableConfigAndButtons()));
	QObject::connect(rbUser2, SIGNAL(clicked()), this, SLOT(enableConfigAndButtons()));
}


TopWidget::~TopWidget(void)
{

}


void TopWidget::resetUI()
{
	// When starting UI for the first time, or selecting the NONE experience, call this.
	std::cout<<"TopWidget::resetUI"<<std::endl;

	disableGbRobotType();
	disableGbNetworkType();
	disableGbUserChoice();
	disableGbLogFile();
	disableButtons();

	// IP and ports stuff 
	gbConfig->setEnabled(false);
}

void TopWidget::experienceChosen(int index){
	std::cout<<"TopWidget::experienceChosen:"<< index <<std::endl;
	resetUI();
	if(index != 0){ // if experience chosen is not NONE
		enableGbRobotType();
	}
}

// ENABLES
void TopWidget::enableGbRobotType(){
	std::cout<<"TopWidget::enableGbRobotType"<<std::endl;
	gbRobot->setEnabled(true);
}

void TopWidget::enableGbNetworkType(){
	std::cout<<"TopWidget::enableGbNetworkType"<<std::endl;
	gbNetworkType->setEnabled(true);
}

void TopWidget::enableGbUserChoice(){
	std::cout<<"TopWidget::enableGbUserChoice"<<std::endl;
	gbUserChoice->setEnabled(true);
}

void TopWidget::enableGbConfig(){
	std::cout<<"TopWidget::enableGbConfig"<<std::endl;
	gbConfig->setEnabled(true);
}

void TopWidget::enableConfigAndButtons(){
	std::cout<<"TopWidget::enableConfigAndButtons"<<std::endl;
	enableGbLog();
	enableGbConfig();
	enableButtons();
}

void TopWidget::enableButtons(){
	std::cout<<"TopWidget::enableButtons"<<std::endl;
	newWindow->bottomWidget->enableButtons();
}

void TopWidget::enableGbLog(){
	std::cout<<"TopWidget::enableGbLog"<<std::endl;
	gbLogFile->setEnabled(true);
}


// DISABLES

void TopWidget::disableGbRobotType(){
	std::cout<<"TopWidget::disableGbRobotType"<<std::endl;
	gbRobot->setEnabled(false);

	rbRobot1->setAutoExclusive(false);
	rbRobot1->setChecked(false);
	rbRobot1->setAutoExclusive(true);

	rbRobot2->setAutoExclusive(false);
	rbRobot2->setChecked(false);
	rbRobot2->setAutoExclusive(true);
}


void TopWidget::disableGbNetworkType(){
	std::cout<<"TopWidget::disableGbNetworkType"<<std::endl;
	gbNetworkType->setEnabled(false);

	rbNetworkType1->setAutoExclusive(false);
	rbNetworkType1->setChecked(false);
	rbNetworkType1->setAutoExclusive(true);

	rbNetworkType2->setAutoExclusive(false);
	rbNetworkType2->setChecked(false);
	rbNetworkType2->setAutoExclusive(true);
}

void TopWidget::disableGbUserChoice(){
	std::cout<<"TopWidget::disableGbUserChoice"<<std::endl;
	gbUserChoice->setEnabled(false);

	rbUser1->setAutoExclusive(false);
	rbUser1->setChecked(false);
	rbUser1->setAutoExclusive(true);

	rbUser2->setAutoExclusive(false);
	rbUser2->setChecked(false);
	rbUser2->setAutoExclusive(true);
}

void TopWidget::disableGbConfig(){
	std::cout<<"TopWidget::disableGbConfig"<<std::endl;
	gbConfig->setEnabled(false);
}

void TopWidget::disableGbLogFile(){
	std::cout<<"TopWidget::disableGbLogFile"<<std::endl;
	gbLogFile->setEnabled(false);
}


void TopWidget::disableButtons(){
	std::cout<<"TopWidget::disableButtons"<<std::endl;
	newWindow->bottomWidget->disableButtons();
}

