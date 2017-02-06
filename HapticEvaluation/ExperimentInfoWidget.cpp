#include "ExperimentInfoWidget.h"


ExperimentInfoWidget::ExperimentInfoWidget(void)
{
	expTitle = new QLabel("Experience Explication");
	teDetails = new QTextEdit();
	teDetails->setReadOnly(true);
	

	lay = new QGridLayout();
	lay->addWidget(expTitle);
	lay->addWidget(teDetails);
	setLayout(lay);
}



ExperimentInfoWidget::~ExperimentInfoWidget(void)
{
}
