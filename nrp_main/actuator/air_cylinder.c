#include <avr/io.h>
#include <actuator.h>
#include "air_cylinder.h"

void a0(enum AirCylinderAction act) {
	PC.Port.Bit.B6 = act;
}


void a1(enum AirCylinderAction act) {
	PC.Port.Bit.B7 = act;
}

AirCylinder airCylinders[] = {
	{.drive = a0},
	{.drive = a1}
};

AirCylinder *openAirCylinder(enum AirCylinderNumber no)
{
	return &airCylinders[no];
}
