#include "ExternalControl.h"
#include <math.h>
/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

const double ExternalControl::F2N_K_PROPORTIONNAL = 0.08; //higher value makes the device vibrate
const double ExternalControl::F2N_K_INTEGRAL = 0; //higher value makes the device stiffer but can become unstable


ExternalControl::ExternalControl()
{
	this->setType(VELOCITY_MODE);
	this->setDataNumber(1);
	//init cpt
	cpt = 0;
	positionIntegrator = Vector3();
	
	
	//square angle init
	squareAngle[0] = Vector3(-80,45,0) ;
	squareAngle[1] = Vector3(80,45,0);
	squareAngle[2] = Vector3(80,-45,0);
	squareAngle[3] = Vector3(-80,-45,0);

	referencePosition = squareAngle[0];
	MAX_SPEED = 500;
	MAX_ACCELERATION = 2000;
	choice = 20;

	/*************time init*****************/
	//Reference time
	t[0] = 10;

	//first line
	t[1] = t[0] + MAX_SPEED/MAX_ACCELERATION;
	t[2] = t[0] + abs(squareAngle[1].x-squareAngle[0].x)/MAX_SPEED;
	t[3] = t[1] + t[2] - t[0];
	t[4] = t[3] + 2; // 2 seconds break time on the edge

	//second line
	t[5] = t[4] + MAX_SPEED/MAX_ACCELERATION;
	t[6] = t[4] + abs(squareAngle[2].y-squareAngle[1].y)/MAX_SPEED;
	t[7] = t[5] + t[6] - t[4];
	t[8] = t[7] + 0;// no break time on this edge
	
	//third line
	t[9] = t[8] + MAX_SPEED/MAX_ACCELERATION;
	t[10] = t[8] + abs(squareAngle[3].x-squareAngle[2].x)/MAX_SPEED;
	t[11] = t[9] + t[10] - t[8];
	t[12] = t[11] + 1; // 1 seconds break time on the edge

	//forth line
	t[13] = t[12] + MAX_SPEED /MAX_ACCELERATION;
	t[14] = t[12] + abs(squareAngle[0].y-squareAngle[3].y)/MAX_SPEED;
	t[15] = t[13] + t[14] - t[12];
	t[16] = t[15] + 5; // 5s break time to finish


}

void ExternalControl::trajectory()
{
	for(int i = 0; i < 16; i++)
	{
		if(t[i] <= cpt*fech && t[i+1] > cpt*fech)
		{
			choice = i;
			break;
		}
	}

	switch(choice%4)
	{
	case 0:
		if((choice-choice%4)/4 == 0 ||(choice-choice%4)/4 == 3 )
			referencePosition[((choice-choice%4)/4)%2] += 1/2*(MAX_ACCELERATION)*(cpt*fech-t[choice])*(cpt*fech-t[choice]); 
		else
			referencePosition[((choice-choice%4)/4)%2] -= 1/2*(MAX_ACCELERATION)*(cpt*fech-t[choice])*(cpt*fech-t[choice]); 
		break;

	case 1 :
		if((choice-choice%4)/4 == 0 ||(choice-choice%4)/4 == 3 )
			referencePosition[((choice-choice%4)/4)%2] += MAX_SPEED*fech;
		else
			referencePosition[((choice-choice%4)/4)%2] -= MAX_SPEED*fech;
		break;
	case 2 : 
		if((choice-choice%4)/4 == 0 ||(choice-choice%4)/4 == 3 )
			referencePosition[((choice-choice%4)/4)%2] += MAX_SPEED*fech - 1/2*(MAX_ACCELERATION)*(cpt*fech-t[choice])*(cpt*fech-t[choice]); 
		else
			referencePosition[((choice-choice%4)/4)%2] -= MAX_SPEED*fech - 1/2*(MAX_ACCELERATION)*(cpt*fech-t[choice])*(cpt*fech-t[choice]); 
		break;
	case 3 : 
		referencePosition = squareAngle[(choice-choice%4)/4+1];
		break;
	}
}

ExternalControl::~ExternalControl()
{

}

void ExternalControl::compute()
{
	trajectory();

	positionIntegrator += (referencePosition-localPosition)*F2N_K_INTEGRAL*fech;
	localAppliedForce = (positionIntegrator + referencePosition-localPosition)*F2N_K_PROPORTIONNAL;

	cpt++;
}

DataType ExternalControl::send()
{
	return sendDataType;
}