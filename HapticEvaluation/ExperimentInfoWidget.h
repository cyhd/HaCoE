#pragma once

#include "QWidget.h"
#include "qlabel.h"
#include "qtextedit.h"
#include "qgridlayout.h"
#include "qstring.h"

class ExperimentInfoWidget : public QWidget
{
public:
	ExperimentInfoWidget(void);
	~ExperimentInfoWidget(void);

	QVBoxLayout* lay;
	QLabel* expTitle;
	QTextEdit *teDetails;
};

