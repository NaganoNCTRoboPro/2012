#include <avr/io.h>
#include <actuator.h>
#include "air_cylinder.h"

void a0(enum AirCylinderAction act) {
	PA.Port.Bit.B0 = act;
}


void a1(enum AirCylinderAction act) {
	PA.Port.Bit.B0 = act;
}

AirCylinder airCylinders[] = {
	{.drive = a0},
	{.drive = a1}
};

AirCylinder *getAirCylinder(enum AirCylinderNumber no)
{
	return &airCylinders[no];
}
