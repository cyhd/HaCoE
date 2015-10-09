#include "OptiTrackDummy.h"

OptiTrackDummy::OptiTrackDummy(void)
{
 // init 
}

OptiTrackDummy::~OptiTrackDummy(void)
{
// close the connection
}

short OptiTrackDummy::readData() 
{
 // read the data from OptiTrack API

 // fill the structure
	this->setTranslation( 3.0f, 4.0f, 7.0f );
	this->setRotation( 3.0f, 4.0f, 7.0f );

	return SUCCESS;
}

void OptiTrackDummy::closeConnection()
{

}
