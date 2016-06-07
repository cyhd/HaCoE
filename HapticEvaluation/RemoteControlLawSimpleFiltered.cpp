#include "RemoteControlLawSimpleFiltered.h"

RemoteControlLawSimpleFiltered::RemoteControlLawSimpleFiltered()
{
	cpt = 0;
	size = 3;
}

RemoteControlLawSimpleFiltered::~RemoteControlLawSimpleFiltered()
{

}

const double RemoteControlLawSimpleFiltered::F2N_K_FORCE = 0.01; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double RemoteControlLawSimpleFiltered::F2N_SCALERFORCE = 1; //SCALERFORCE is used to modify the stiffness difference between the two different haptic devices


Vector3 RemoteControlLawSimpleFiltered::getForce(Vector3 transA, Vector3 transB )
{
		
		forceControl[cpt].x = -F2N_K_FORCE*( transA.x - transB.x ); 
		forceControl[cpt].y = -F2N_K_FORCE*( transA.y - transB.y );
		forceControl[cpt].z = -F2N_K_FORCE*( transA.z - transB.z );
		
		forceControl[cpt].x = forceControl[cpt].x * F2N_SCALERFORCE;
		forceControl[cpt].y = forceControl[cpt].y * F2N_SCALERFORCE;
		forceControl[cpt].z = forceControl[cpt].z * F2N_SCALERFORCE;
			
		cpt++;
		if (cpt=size) {cpt=0;}
		
		for(int i = 0; i < size; i++) 
		{
			forceControlA.x += forceControl[i].x;
			forceControlA.y += forceControl[i].y;
			forceControlA.z += forceControl[i].z;
		}

		forceControlA.x /= size;
		forceControlA.y /= size;
		forceControlA.z /= size;

		return forceControlA;
}