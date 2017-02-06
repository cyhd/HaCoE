
#include "CenterWidget.h"


CenterWidget::CenterWidget(QWidget *parent) : QWidget(parent)
{
	teExpDetails = new QTextEdit();
	lExperimentInfo = new QLabel("Experiment details");
	lay = new QGridLayout(this);
	lay->addWidget(lExperimentInfo,0,0);
	lay->addWidget(teExpDetails,1,0);
	
	setLayout(lay);
}


CenterWidget::~CenterWidget(void)
{

}