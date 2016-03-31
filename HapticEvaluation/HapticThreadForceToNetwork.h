#pragma once
#include <qthread.h>
#include "HapticThread.h"

class HapticThreadForceToNetwork : public HapticThread
{
public:
	HapticThreadForceToNetwork( void );
	~HapticThreadForceToNetwork( void );
	void run();
private:
	Vector3 transA; //translational position of haptic device
	Vector3 transB;
	Matrix3x3 rotA; //rotational position of haptic device
	Matrix3x3 rotB;
	double t2; //used in calculating the cross product to find how much the angle is deviated.
	double t5;
	double t10;
	static const double DF_GENERAL_DAMPING_TRANSLATION;
	static const double DF_GENERAL_DAMPING_ROTATION;
	static const double DF_K_FORCE;
	static const double DF_K_TORQUE;
	static const double DF_SCALERFORCE;
};