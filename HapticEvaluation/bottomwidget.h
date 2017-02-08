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
	QPushButton *pbDeactivate;
	QPushButton *pbZero;
	QPushButton *pbConnect;
	QPushButton *pbDisconnect;
	QPushButton *pbHelp;
	QLabel *lLog;
	QTextEdit *tLog;

	QStackedWidget *swActivateStack;
	QStackedWidget *swConnectStack;

	// PREVIOUS STUFF
	QString filename;
	QString date;
	QDateTime *dateTime;

	// Methods
	void startLogging();
	void stopLogging();

public slots:
	void connectPbActivate();
	void connectPbDeactivate();
	void connectPbConnect();
	void connectPbDisconnect();
	void connectPbStart();
	void connectPbStop();

	void activateProcess();
	void connectProcess();
	void disconnectProcess();
	void startProcess();
	void stopProcess();
	
	void enableActivate();
	void enableConnect();
	void enableStart();
	void enableStop();

	void disableActivate();
	void disableConnect();
	void disableStart();
	void disableButtons();
	void disableStop();

	void setActivateButton();
	void setDeactivateButton();

	void setConnectButton();
	void setDisconnectButton();
	
	void activateCorrectDevice();
	void deactivateCorrectDevice();
	//void setStopLog();
	//void zeroB();
};

#endif





