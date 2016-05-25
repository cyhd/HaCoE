#include "haptlinksupervisor.h"
#include "OmniDevice.h"
#include "HapticThreadForceToNetwork.h"

HapticThreadForceToNetwork :: HapticThreadForceToNetwork( void ){}

HapticThreadForceToNetwork :: ~HapticThreadForceToNetwork( void ){}

const double HapticThreadDoubleForce::DF_K_FORCE = 0.08; //K_FORCE and K_TORQUE are used to adjust the tightness of the control.  Higher values are more unstable
const double HapticThreadDoubleForce::DF_SCALERFORCE = 1; //SCALERFORCE is used to modify the stiffness difference between the two different haptic devices

void HapticThreadForceToNetwork::run()
{
	HaptLinkSupervisor *supervisor=HaptLinkSupervisor::getInstance();
	haptDeviceA = supervisor->getHaptDeviceA();
	haptDeviceB = supervisor->getHaptDeviceB();

	zero.x = 0;
	zero.y = 0;
	zero.z = 0;

	/*
	RemoteControl *command:
	command = new RemoteControlLaw1(...); 
	threadW = new WriteNetworkThread(command);
	threadR = new ReadNetworkThread(command);

	public class RemoteControl {

	  public Vector3 transformR(Vector3 pos, Vector3 force, float lamba) = 0;
	  public Vector3 transformW(Vector3 pos, Vector3 force, float lamba) = 0;

	
	public RemoteControlLaw1 : public RemoteControl { 
	
	public Vector3 transformR(Vector3 pos, Vector3 force, float lamba) 
	 {
	    Vector3 ret = pos * lambda *foce;
		return ret;
	 }

	  public Vector3 transformW(Vector3 pos, Vector3 force, float lamba) = 0;

	
	}


	*/
	while( supervisor->getThreadStarted() )
	{
		//This block is for dual link control (master-master) in force mode
		haptDeviceA->readData();
		
		supervisor->getMutex()->lock(); // start mutex
		transA = haptDeviceA->getTranslation();
		transB = haptDeviceB->getTranslation();
		supervisor->getMutex()->unlock();//end mutex

		forceControlA.x = -DF_K_FORCE*( transA.x - transB.x ); 
		forceControlA.y = -DF_K_FORCE*( transA.y - transB.y );
		forceControlA.z = -DF_K_FORCE*( transA.z - transB.z );
		
		forceControlA.x = forceControlA.x * DF_SCALERFORCE;
		forceControlA.y = forceControlA.y * DF_SCALERFORCE;
		forceControlA.z = forceControlA.z * DF_SCALERFORCE;
		
		haptDeviceA->writeForce( forceControlA , torqueControlA );
		
		usleep( sleepTime );
	}
}