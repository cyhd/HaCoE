#include "Ati.h"
#include "ljackuw.h"
#pragma comment(lib, "ljackuw.lib")


const int AVERAGE_NUM = 50;

Ati::Ati(void)
{
	//init

	//SI
	nano17Init("FT9498.cal");

	//US
	//nano17Init("FT7816.cal");
	
}

Ati::~Ati(void)
{
	// close the connection
	closeConnection();
	
}
void Ati::closeConnection(void)
{
	destroyCalibration(cal);
}

short Ati::readData()
{
 // read the data from ATI API

	setFT();

	this->setTranslation( FT[0], FT[1], FT[2] );
	this->setRotation( FT[3], FT[4], FT[5] );

	return SUCCESS;
}

void Ati::setVoltage(void)
{
	int i = 0;
	long errorcode;
	long idnum=-1;
	long demo=0;
	long stateIO=0;
	long numCh=4;
	long channel_1[4]={0,1,2,3};
	long channel_2[2]={4,5};
	long gains_1[4]={0,0,0,0};
	long gains_2[2]={0,0};
	long ov;
    float voltages_1[4];
	float voltages_2[4];

	for (i=0;i<4;i++)
	{
		voltages_1[i] = 0;
		voltages_2[i] = 0;
	}

	errorcode = AISample(&idnum,demo,&stateIO,0,1,4,channel_1,gains_1,0,&ov,voltages_1);
/*
	if(errorcode != 0)
	{
		cout << "Error Code 1 = " << errorcode << "\n";
		exit(1);		
	}
*/
    errorcode = AISample(&idnum,demo,&stateIO,0,1,2,channel_2,gains_2,0,&ov,voltages_2);
/*    
	if(errorcode != 0)
	{
		cout << "Error Code 2 = " << errorcode << "\n";
		exit(1);		
	}
*/
	voltages[0] = voltages_1[0];
	voltages[1] = voltages_1[1];
	voltages[2] = voltages_1[2];
	voltages[3] = voltages_1[3];
	voltages[4] = voltages_2[0];
	voltages[5] = voltages_2[1];
}

void Ati::setFT(void)
{
	setVoltage();
	ConvertToFT(cal,voltages,FT);

}
void Ati::nano17Init(char *filename)
{
	float Total[6];
    unsigned short index = 1;   // index of calibration in file (second parameter; default = 1)	

	cal=createCalibration(filename,index);

//	Taking Average samples for bias
	for(int j=0;j<AVERAGE_NUM;j++)
	{
		setVoltage();

		for(int k=0;k<6;k++)
			Total[k]=Total[k]+voltages[k];
	}
	
	for(int k=0;k<6;k++)
     Total[k]=Total[k]/AVERAGE_NUM;

	Bias(cal,Total);
}