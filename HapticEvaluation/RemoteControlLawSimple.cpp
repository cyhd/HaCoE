#include "RemoteControlLawSimple.h"

/*****************************************************************************
Simple control law : get the mean value between local and remote Omni and 
gets the mean value. Then calculate the forces to flush to the local Omni
via F2N_K_Force
*****************************************************************************/

RemoteControlLawSimple::RemoteControlLawSimple()
{

}

RemoteControlLawSimple::~RemoteControlLawSimple()
{

}

const double RemoteControlLawSimple::F2N_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double RemoteControlLawSimple::F2N_SCALERFORCE = 1; //SCALERFORCE is used to modify the stiffness difference between the two different haptic devices


Vector3 RemoteControlLawSimple::getForce(Vector3 transA, Vector3 transB )
{
	
		forceControlA.x = -F2N_K_FORCE*( transA.x - transB.x ); 
		forceControlA.y = -F2N_K_FORCE*( transA.y - transB.y );
		forceControlA.z = -F2N_K_FORCE*( transA.z - transB.z );
		

		//not used since F2N_SCALERFORCE = 1
		forceControlA.x = forceControlA.x * F2N_SCALERFORCE;
		forceControlA.y = forceControlA.y * F2N_SCALERFORCE;
		forceControlA.z = forceControlA.z * F2N_SCALERFORCE;

		return forceControlA;
}