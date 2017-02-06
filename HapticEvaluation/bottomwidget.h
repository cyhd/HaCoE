#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>

#include <qwidget.h>
#include <qlabel.h>
#include <QHBoxLayout>
#include <qpushbutton.h>
#include "newwindow.h"

class NewWindow;
class BottomWidget : public QWidget
{
	Q_OBJECT;

public:
	BottomWidget(NewWindow *parent = NULL);
	~BottomWidget(void);

	NewWindow* newWindow;

	// UI
	QHBoxLayout *lay;
	QPushButton *pbGraphForce;
	QPushButton *pbStop;
	QPushButton *pbStart;
	QPushButton *pbZero;
	QPushButton *pbConnect;
	QPushButton *pbHelp;

	
	// PREVIOUS STUFF
	QString filename;
	QString date;
	QDateTime *dateTime;

	// Methods
	void startLogging();
	void activateCorrectDevice();
	void deactivateCorrectDevice();
	void stopLogging();

public slots:
	void startProcess();
	void stopProcess();
	void initCommunication();
	void disableButtons();
	void enableButtons();
	//void setStopLog();
	//void zeroB();
};

#endif





