#ifndef BOTTOMWIDGET_H
#define BOTTOMWIDGET_H

#include <QWidget>

#include <qwidget.h>
#include <qlabel.h>
#include <QGridLayout>
#include <qpushbutton.h>
#include <qtextedit.h>
#include <QLabel>
#include "newwindow.h"
#include <qstackedwidget.h>
#include <qdialog.h>


class NewWindow;
class BottomWidget : public QWidget
{
	Q_OBJECT;

public:
	BottomWidget(NewWindow *parent = NULL);
	~BottomWidget(void);

	NewWindow* newWindow;

	// UI
	QGridLayout *lay;
	QPushButton *pbGraphForce;
	QPushButton *pbStop;
	QPushButton *pbStart;
	QPushButton *pbActivate;
	QPushButton *pbZero;
	QPushButton *pbCalibrate;
	QPushButton *pbConnect;
	QPushButton *pbHelp;
	QLabel *lLog;
	QTextEdit *teLog;

	// PREVIOUS STUFF
	QString filename;
	QString date;
	QDateTime *dateTime;

	// Methods
	void startLogging();
	void stopLogging();
	
	void setStatus(char *s);
	
	void connectPbActivate();
	void connectPbConnect();
	void connectPbStart();
	void connectPbStop();
	void connectPbCalibrate();

public slots:

	void activateProcess();
	void connectProcess();
	void startProcess();
	void stopProcess();
	
	void enableActivate();
	void enableConnect();
	void enableStart();
	void enableCalibrate();
	void enableStop();

	void disableActivate();
	void disableConnect();
	void disableStart();
	void disableCalibrate();
	void disableButtons();
	void disableStop();
	void disableZero();
	
	void activateCorrectDevice();
	void deactivateCorrectDevice();
	void calibrateCorrectDevice();
	//void setStopLog();
	//void zeroB();

	void showHelpDialog();
};

#endif





