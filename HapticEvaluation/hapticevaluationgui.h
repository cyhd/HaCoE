//
// HaCoE Haptic Communication Evaluation is a software to build haptic communication experiments, using haptic arms and force sensors, generating standard XML files you can load in R.
//
//   Copyright (C) 2014 Cedric Dumas 
//
//   contributors: Cedric Dumas, Boris Takac, Devin Luu, Amine Chellali, Caroline Cao
//   contact: Cedric.Dumas@gmail.com
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
//

#ifndef HAPTICEVALUATIONGUI_H
#define HAPTICEVALUATIONGUI_H

//#include <QtGui/QMainWindow>
#include <QWidget>
#include "ui_hapticevaluationgui.h"
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

class HapticEvaluationGUI : public QMainWindow, Observer
{
	Q_OBJECT

public:
	HapticEvaluationGUI(QWidget *parent = 0, Qt::WFlags flags = 0);
	~HapticEvaluationGUI();

    //QwtPlotCurve  * courbePx;
	QwtPlotCurve  * courbeFxA;
	QwtPlotCurve  * courbeFyA;
	QwtPlotCurve  * courbeFzA;
    QwtPlotCurve  * courbeFtotalA;
	QwtPlotCurve  * courbeFxB;
	QwtPlotCurve  * courbeFyB;
	QwtPlotCurve  * courbeFzB;
    QwtPlotCurve  * courbeFtotalB;
	QwtPlotMarker * marker;

	//bool calSI;					//calibration file flag

	static HapticEvaluationGUI* getInstance();		
	virtual void update(short value);				//inherited from Observer
	
	void updateDevForceDisplay(Vector3 f1 , Vector3 f2); //One vector per ATI sensor
	//void updateDevPositionDisplay(Vector3 p);
	//void updateDevOrientationDisplay(Vector3 o);
	void updateDevTorqueDisplay(Vector3 t1 , Vector3 t2);
	void updateDevGraphDisplay(Vector3 g , Vector3 p);
	void updateCheckGraph();
	//QString getKey(){return keyString;}

	bool getAtiAActivate(); //gets whether or not the user wants to turn on each ATI sensor
	bool getAtiBActivate();
	bool getEntactAActivate(); //gets whether or not the user wants to turn on each entact
	bool getEntactBActivate();

	outputSide getPref() const; //gets hand dominance
  
	void updateInterfaceGriffith(expType type); // ui patch for Griffith CST and Linear mode
	void updateGriffith(Vector3 posB, Vector3 forceB);

public slots:
	//setters: setting variable on predefined value upon signal
	void setStartLog();
	void setStopLog();
	void switchAtiA( void ); //switch on/off the ATI associated with each check box
	void switchAtiB( void );
	void switchEntactA( void );
	void switchEntactB( void );
	void calibrateEntactA( void );
	void calibrateEntactB( void );
	void setExpInfo( void );
	void setHRExpFile( void );
	void readNextHRTrial( void );
	void setDominantHand( void );
	void setGoTo( void );
	//void setupDevice();
	void zeroA(void );
	void zeroB(void );

private:

	Ui::HapticEvaluationGUIClass ui;

	//QButtonGroup *sexgroup;
	QButtonGroup *prefgroup;
	QButtonGroup *expgroup;
	QButtonGroup *haptrepoutputsidegroup;

	QDateTime *dateTime;
/********************************************************* Plot Stuff**********/
	int      duree;
	int      period;
	int      nbPoints;
	int      index;
	//int      index2;
	//int      index3;
	QwtPlotZoomer *zoomer;
	QwtPlotGrid *Grid;
	int      realIndex;
	double * dataX;
	double * dataFxA;
	double * dataFyA;
	double * dataFzA;
	double * dataFtotalA;

	double * dataFxB;
	double * dataFyB;
	double * dataFzB;
	double * dataFtotalB;

	//double * dataPx;
    
	double * timeChannel;
	double * yChannel;
	bool     state;
	bool     enableY;

	QString filename;
	//QString sex;
	QString pref;
	//QString age;
	//QString keyString;
	QString date;
	
	//void keyPressEvent(QKeyEvent * event);
	static HapticEvaluationGUI *instance;
	void initGraph();
	void keyPressEvent(QKeyEvent *event);
	void disableHRFrame();
	void enableHRFrame();

	void enableDepthFrame();
	void disableDepthFrame();

	void disableInterface();
	void enableInterface();

	void showDataA();
	void hideDataA();
	void showDataB();
	void hideDataB();
		
	bool forceActiveA; //A  and B activated status for ati
	bool forceActiveB;

	void setHRParameterDisplay();

	public:
		
	void setStatus(char *s) { ui.labelStatus->setText(s); }

};

#endif // HAPTICEVALUATIONGUI_H
