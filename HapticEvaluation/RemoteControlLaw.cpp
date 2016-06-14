
#include "RemoteControlLaw.h"

void RemoteControlLaw::setData(Vector3 data, DataType dataType)
{
	switch(dataType)
	{
	case LOCAL_POSITION :
		localPosition = data;
		break;
	case REMOTE_POSITION :
		remotePosition = data;
		break;
	case LOCAL_FORCE :
		localForce = data;
		break;
	case REMOTE_FORCE :
		remoteForce = data;
		break;
	case LOCAL_VELOCITY :
		localVelocity = data;
		break;
	case REMOTE_VELOCITY :
		remoteVelocity = data;
		break;
	case DESIRED_LOCAL_VELOCITY :
		desiredLocalVelocity = data;
		break;
	case DESIRED_REMOTE_VELOCITY :
		desiredRemoteVelocity = data;
		break;
	}
}

Vector3 RemoteControlLaw::getData(DataType dataType)
{
	switch(dataType)
	{
	case LOCAL_POSITION :
		return localPosition;
		break;
	case REMOTE_POSITION :
		return remotePosition;
		break;
	case LOCAL_FORCE :
		return localForce;
		break;
	case REMOTE_FORCE :
		return remoteForce;
		break;
	case LOCAL_VELOCITY :
		return localVelocity;
		break;
	case REMOTE_VELOCITY :
		return remoteVelocity;
		break;
	case DESIRED_LOCAL_VELOCITY :
		return desiredLocalVelocity;
		break;
	case DESIRED_REMOTE_VELOCITY :
		return desiredRemoteVelocity;
		break;
	}
}


