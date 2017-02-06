#include "newwindow.h"

NewWindow::NewWindow(QWidget *parent): QMainWindow(parent)
{

	// Bottom Widget Stuff
	bottomWidget = new BottomWidget(this);
	
	// TopWidget Stuff
	topWidget = new TopWidget(this);

	// Center Widget Stuff
	centerWidget = new QStackedWidget();

	expNONE = new ExperienceNONE();
	expATI = new ExperienceATI();
	expSLC = new ExperienceSLinkControl();
	expSLF = new ExperienceSLinkForce();
	expDLF = new ExperienceDLinkForce();
	expHRepS = new ExperienceHapticRepSimul();
	expHRSeq = new ExperienceHapticRepSeq();
	expHRA = new ExperienceHapticRepAuto();
	expSFF = new ExperienceSingleForceFeed();
	expFDC = new ExperienceForceDepthC();
	expFDL = new ExperienceForceDepthL();
	expFTN = new ExperienceForceToNetwork();
	
	centerWidget->addWidget(expNONE);
	centerWidget->addWidget(expATI);
	centerWidget->addWidget(expSLC);
	centerWidget->addWidget(expSLF);
	centerWidget->addWidget(expDLF);
	centerWidget->addWidget(expHRepS);
	centerWidget->addWidget(expHRSeq);
	centerWidget->addWidget(expHRA);
	centerWidget->addWidget(expSFF);
	centerWidget->addWidget(expFDC);
	centerWidget->addWidget(expFDL);
	centerWidget->addWidget(expFTN);

	QObject::connect(topWidget->cbExperience, SIGNAL(currentIndexChanged(int)), centerWidget, SLOT(setCurrentIndex(int)));
	
	// One day, uncomment this for the graphs to work, I thnk
	// HaptLinkSupervisor::getInstance()->attachObserver(this);

	// Configure robots stuff
	HaptLinkSupervisor::getInstance()->setLJActiveA(false);
	HaptLinkSupervisor::getInstance()->setLJActiveB(false);
	HaptLinkSupervisor::getInstance()->setHaptActiveA(false);
	HaptLinkSupervisor::getInstance()->setHaptActiveB(false);
	DataLogger::getInstance()->setDataActiveA(false);
	DataLogger::getInstance()->setDataActiveB(false);
	DataLogger::getInstance()->setCommandActive(false);
	DataLogger::getInstance()->setExternalCommandActive(false);
	DataLogger::getInstance()->setHapticActiveA(false);
	DataLogger::getInstance()->setHapticActiveB(false);
	

	// WINDOW WIDGET STUFF
	lay = new QVBoxLayout();
	lay->addWidget(topWidget);
	lay->addWidget(centerWidget);
	lay->addWidget(bottomWidget);

	window = new QWidget();
	window->setLayout(lay);


	this->setCentralWidget(window);

}

NewWindow::~NewWindow()
{

}


