#include "AtiDummy.h"

AtiDummy::AtiDummy(char *filename)
{
	//init
}

AtiDummy::~AtiDummy(void)
{
	// close the connection
}

short AtiDummy::readData()
{
 // read the data from ATI API


 // fill the structure
	this->setTranslation( 1.0f, 2.0f, 3.0f );
	this->setRotation( 4.0f, 5.0f, 6.0f );

	return SUCCESS;
}

void AtiDummy::closeConnection()
{

}