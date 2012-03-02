#ifndef AIR_CYLINDER_H
#define AIR_CYLINDER_H

enum AirCylinderNumber {
	AirCylinder0 = 0,
	AirCylinder1
};

enum AirCylinderAction {
	OFF = 0,
	ON
};

typedef struct st_air_Cylinder {
	void (*drive)(enum AirCylinderAction act);
} AirCylinder;


extern AirCylinder *openAirCylinder(enum AirCylinderNumber no);
#endif
