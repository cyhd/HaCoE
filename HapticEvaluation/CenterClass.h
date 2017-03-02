#pragma once
#include <qobject.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qradiobutton.h>
#include <qtextedit.h>

class CenterWidget
{
	Q_OBJECT

public:
	CenterWidget(void);
	
	QLabel experienceLabel;
	QLabel whoIAmLabel;
	QLabel robotTypeLabel;
	QComboBox experienceComboBox;
	QComboBox robotTypeComboBox;
	QRadioButton jamesButton; 
	QRadioButton bondButton;

	QLabel IPJamesLabel;
	QLabel IPBondLabel;
	QLabel jamesPortLabel;
	QLabel bondPortLabel;
	QTextEdit jamesIPText;
	QTextEdit bondIPText;
	QTextEdit jamesPortText;
	QTextEdit bondPortText;

	QLabel logFileLabel;
	QTextEdit logFileText;
	
	
};

