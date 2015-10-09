#include "Device.h"
#include "haptlinksupervisor.h"
#include "EntactDevice.h"
#include "HapticThreadRepSeq.h"

void HapticThreadRepSeq::setK_FORCE( double k )
{
	RS_K_FORCE = k;
}
void HapticThreadRepSeq::setK_TORQUE( double k )
{
	RS_K_TORQUE = k;
}