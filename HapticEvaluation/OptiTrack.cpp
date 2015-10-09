#include "OptiTrack.h"


OptiTrack::OptiTrack(void)
{
	cameraInit("Project.ttp");
}
OptiTrack::~OptiTrack(void)
{
	closeConnection();
}

void OptiTrack::closeConnection(void ) 
{
	TT_Shutdown();
}

void OptiTrack::cameraInit(char *project)
{
	TT_Initialize();    
    TT_LoadProject(project);
}

void OptiTrack::setPO(void)
{
	result = TT_Update();
      
	if(result == NPRESULT_SUCCESS)
	{		
	 	
	 if(TT_TrackableCount()>0)
	 {
		TT_TrackableLocation(0,&x,&y,&z,&qx,&qy,&qz,&qw,&yaw,&pitch,&roll);
		
		if(TT_IsTrackableTracked(0) == 0)
		{
		 x = 9999;
		 y = 9999;
		 z = 9999;

		 yaw = 9999;
		 pitch = 9999;
		 roll = 9999;		
		}
	 }
		
	}	
}

short OptiTrack::readData()
{
	//this->setTranslation( 13.0f, 14.0f, 17.0f );
	//this->setRotation( 13.0f, 14.0f, 17.0f );
	setPO();

	this->setTranslation( x, y, z );
	this->setRotation( yaw, pitch, roll );

	return SUCCESS;
}