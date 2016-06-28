
#include "RemoteControlLaw.h"

RemoteControlLaw::RemoteControlLaw()
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 2000; j++)
			delayedDataBuff[i][j] = Vector3(0.0,0.0,0.0);
		delayBuffCpt[i]=0;
	}
}

RemoteControlLaw::~RemoteControlLaw()
{
}

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

void RemoteControlLaw::setTimeDelay (int timeDelay)
{
	delayValue = timeDelay;
}

Vector3 RemoteControlLaw::delay(Vector3 data, DataType type)
{
	delayedDataBuff[type][delayBuffCpt[type]] = data;
	delayBuffCpt[type]++;
	if (delayBuffCpt[type] >= delayValue)
		delayBuffCpt[type] = 0;
	return delayedDataBuff[type][delayBuffCpt[type]];
}

Vector3 RemoteControlLaw::saturation(Vector3 data, double saturationValue)
{
	for (int i = 0; i < 3; i++)
	{
		if(data[i] > saturationValue || data[i] < -saturationValue)
		{
			data[i] = ((data[i] > 0) - (data[i] < 0))*saturationValue;
		}
	}
	
	return data;
}

Vector3 RemoteControlLaw::hysteresis(Vector3 data, double hysteresisValue)
{
	for (int i = 0; i < 3; i++)
	{
		if(data[i] < hysteresisValue && data[i] > -hysteresisValue)
		{
			data[i] = 0;
		}
	}
	return data;
}

