#include "topwidget.h"

TopWidget::TopWidget(NewWindow *parent) : QWidget(parent)
{
	newWindow = parent;
	initUI();
}

void TopWidget::initUI(){
	// Experience
	gbExperience = new QGroupBox("1- Choose Experience");
	QGridLayout *layExperience = new QGridLayout();
	lbExperience = new QLabel("Experience: ");
	cbExperience = new QComboBox();
	// Add experiences name in combobox
	for(int i = 0; i < newWindow->centerWidget->count(); i++){
		ExperienceWidget* exp = qobject_cast<ExperienceWidget*>(newWindow->centerWidget->widget(i));
		cbExperience->addItem(QString::fromStdString(exp->name));
	}

	/*
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
		*/
	layExperience->addWidget(lbExperience, 0, 0);
	layExperience->addWidget(cbExperience, 0, 1);
	gbExperience->setLayout(layExperience);


	// Robot type
	robot1name = "Omni";
	robot2name = "Phantom X alpha 12";
	gbRobot = new QGroupBox("2- Robot: ");
	rbRobot1 = new QRadioButton(QString::fromStdString(robot1name));
	rbRobot2 = new QRadioButton(QString::fromStdString(robot2name));

	QVBoxLayout *layRobot = new QVBoxLayout();
    layRobot->addWidget(rbRobot1);
    layRobot->addWidget(rbRobot2);
    gbRobot->setLayout(layRobot);

	// User choice
	user1name = "James";
	user2name = "Bond";
	gbUserChoice = new QGroupBox("3- Who am I ?");
	rbUser1 = new QRadioButton(QString::fromStdString(user1name));
	rbUser2 = new QRadioButton(QString::fromStdString(user2name));

	QVBoxLayout *layUserChoice = new QVBoxLayout();
    layUserChoice->addWidget(rbUser1);
    layUserChoice->addWidget(rbUser2);
    gbUserChoice->setLayout(layUserChoice);

	// IP and Port and delay
	gbConfig = new QGroupBox("4- Configuration");
	lbIP1 = new QLabel(QString::fromStdString("IP " + user1name + " : "));
	teIP1 = new QLineEdit("193.54.76.162");
	lbPort1 = new QLabel(QString::fromStdString("Port " + user1name + " :"));
	tePort1 = new QLineEdit("7172");
	lbIP2 = new QLabel(QString::fromStdString("IP " + user2name + " : "));
	teIP2 = new QLineEdit("132.206.74.215");
	lbPort2 = new QLabel(QString::fromStdString("Port " + user2name + " :"));
	tePort2 = new QLineEdit("7174");
	// Delay text field
	lbDelay = new QLabel("Delay:");
	teDelay = new QLineEdit("0");

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
	gbLogFile = new QGroupBox("5- Optional");
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
	lay->addWidget(gbUserChoice, 1, 0, 1, 3);
	lay->addWidget(gbConfig, 1, 3);
	lay->addWidget(gbLogFile, 2, 0, 1, 4);

	setLayout(lay);


	// CONNECTS
	QObject::connect(cbExperience, SIGNAL(currentIndexChanged(int)), this, SLOT(experienceChosen(int)));
	QObject::connect(rbRobot1, SIGNAL(clicked()), this, SLOT(enableGbUserChoice()));
	QObject::connect(rbRobot2, SIGNAL(clicked()), this, SLOT(enableGbUserChoice()));

	QObject::connect(rbUser1, SIGNAL(clicked()), this, SLOT(enableConfigAndButtons()));
	QObject::connect(rbUser2, SIGNAL(clicked()), this, SLOT(enableConfigAndButtons()));
	QObject::connect(rbUser1, SIGNAL(clicked()), this, SLOT(enableCenterWidget()));
	QObject::connect(rbUser2, SIGNAL(clicked()), this, SLOT(enableCenterWidget()));
}


TopWidget::~TopWidget(void)
{

}


void TopWidget::resetUI()
{
	// When starting UI for the first time, or selecting the NONE experience, call this.
	std::cout<<"TopWidget::resetUI"<<std::endl;

	disableGbRobotType();
	disableGbUserChoice();
	disableGbLogFile();
	disableButtons();
	disableCenterWidget();
	// IP and ports stuff 
	gbConfig->setEnabled(false);
}

void TopWidget::experienceChosen(int index){
	std::cout<<"TopWidget::experienceChosen:"<< index <<std::endl;
	setStatus("Experience \"" + newWindow->getExperienceName(index) + "\" chosen.");
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

void TopWidget::enableGbUserChoice(){
	std::cout<<"TopWidget::enableGbUserChoice"<<std::endl;
	std::string robotName;
	
	if(rbRobot1->isChecked()){
		robotName = robot1name;
	} else {
		robotName = robot2name;
	}
	setStatus("\"" + robotName + "\" robot chosen");
	gbUserChoice->setEnabled(true);
}

void TopWidget::enableGbConfig(){
	std::cout<<"TopWidget::enableGbConfig"<<std::endl;
	gbConfig->setEnabled(true);
}

void TopWidget::enableConfigAndButtons(){
	std::cout<<"TopWidget::enableConfigAndButtons"<<std::endl;
	std::string selectedUser;
	if(rbUser1->isChecked()){
		selectedUser = user1name;
	}
	else {
		selectedUser = user2name;
	}
	setStatus("User \"" + selectedUser + "\" selected.");
	enableGbLog();
	enableGbConfig();
	enableActivate();
}

void TopWidget::enableActivate(){
	std::cout<<"TopWidget::enableActivate"<<std::endl;
	newWindow->bottomWidget->enableActivate();
}

void TopWidget::enableGbLog(){
	std::cout<<"TopWidget::enableGbLog"<<std::endl;
	gbLogFile->setEnabled(true);
}

void TopWidget::enableCenterWidget(){
	std::cout<<"TopWidget::enableCenterWidget"<<std::endl;
	newWindow->centerWidget->setEnabled(true);
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

void TopWidget::disableCenterWidget(){
	std::cout<<"TopWidget::disableCenterWidget"<<std::endl;
	newWindow->centerWidget->setEnabled(false);
}


void TopWidget::setStatus(string s){
	newWindow->bottomWidget->setStatus(s);
}