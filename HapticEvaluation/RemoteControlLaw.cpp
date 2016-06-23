
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
	case DESIRED_LOCAL_POSITION :
		desiredLocalPosition = data;
		break;
	case DESIRED_REMOTE_POSITION :
		desiredRemotePosition = data;
		break;
	case LOCAL_APPLIED_FORCE :
		localAppliedForce = data;
		break;
	case REMOTE_APPLIED_FORCE :
		remoteAppliedForce = data;
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
	case DESIRED_LOCAL_POSITION :
		return desiredLocalPosition;
		break;
	case DESIRED_REMOTE_POSITION :
		return desiredRemotePosition;
		break;
	case LOCAL_APPLIED_FORCE :
		return localAppliedForce;
		break;
	case REMOTE_APPLIED_FORCE :
		return remoteAppliedForce;
		break;
	}
}

void RemoteControlLaw::setSampleTime(int sampleTime)
{
	fech = ((double)sampleTime)/1000000.0;
}


Vector3 RemoteControlLaw::saturation(Vector3 data, double saturationValue)
{
	
	if(data.x > saturationValue || data.x < -saturationValue)
	{
		data.x = ((data.x > 0) - (data.x < 0))*saturationValue;
	}


	if(data.y > saturationValue || data.y < -saturationValue)
	{
		data.y = ((data.y > 0) - (data.y < 0))*saturationValue;
	}


	if(data.z > saturationValue || data.z < -saturationValue)
	{
		data.z = ((data.z > 0) - (data.z < 0))*saturationValue;
	}

	return data;
}

Vector3 RemoteControlLaw::hysteresis(Vector3 data, double hysteresisValue)
{
	if(data.x < hysteresisValue && data.x > -hysteresisValue)
	{
		data.x = 0;
	}

	if(data.y < hysteresisValue && data.y > -hysteresisValue)
	{
		data.y = 0;
	}

	if(data.z < hysteresisValue && data.z > -hysteresisValue)
	{
		data.z = 0;
	}
	
	return data;
}

