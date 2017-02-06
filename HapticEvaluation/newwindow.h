#ifndef NEWWINDOW_H
#define NEWWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <TopWidget.h>
#include <CenterWidget.h>
#include <BottomWidget.h>
#include <QLabel>
#include "ExperienceNONE.h"
#include "ExperienceATI.h"
#include "ExperienceSLinkControl.h"
#include <qstackedwidget.h>
#include "ExperienceSLinkForce.h"
#include "ExperienceDLinkForce.h"
#include "ExperienceHapticRepSimul.h"
#include "ExperienceHapticRepSeq.h"
#include "ExperienceHapticRepAuto.h"
#include "ExperienceSingleForceFeed.h"
#include "ExperienceForceDepthC.h"
#include "ExperienceForceDepthL.h"
#include "ExperienceForceToNetwork.h"
#include <string>

#include "Observer.h"
#include "datamodel.h"
#include <math.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_grid.h>
#include <QKeyEvent>
#include <QDateTime>

class TopWidget;
class BottomWidget;
class NewWindow : public QMainWindow //, Observer
{
	Q_OBJECT

public:
	NewWindow(QWidget *parent=0);
	~NewWindow();
	QStackedWidget *centerWidget;

	QWidget *window;
	QVBoxLayout *lay;
	TopWidget *topWidget;
	BottomWidget *bottomWidget;

	ExperienceNONE *expNONE;
	ExperienceATI *expATI;
	ExperienceSLinkControl *expSLC;
	ExperienceSLinkForce *expSLF;
	ExperienceDLinkForce *expDLF;
	ExperienceHapticRepSimul *expHRepS;
	ExperienceHapticRepSeq *expHRSeq;
	ExperienceHapticRepAuto *expHRA;
	ExperienceSingleForceFeed *expSFF;
	ExperienceForceDepthC *expFDC;
	ExperienceForceDepthL *expFDL;
	ExperienceForceToNetwork *expFTN;


};

#endif // NEWWINDOW_H
