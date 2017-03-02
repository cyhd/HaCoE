#ifndef NEWINTERFACE_H
#define NEWINTERFACE_H

#include <QMainWindow>
#include <QWidget>

#include "ui_hapticevaluationgui.h"

class NewInterface : public QMainWindow
{
	
	Q_OBJECT
	
	//Ui::HapticEvaluationGUIClass ui;

public:
	 NewInterface(QWidget *parent =0);
	 ~NewInterface() {}

};

#endif // NEWINTERFACE_H