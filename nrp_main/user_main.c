#include <remote_control_rx.h>
#include <motor.h>
#include <air_cylinder.h>
#include <led.h>
#include <wait.h>

#include <stdbool.h>

#include "remote_control_rx.h"
#if CONTROLLER == SIXAXIS
	#include "rc_sixaxis.h"
#elif CONTROLLER == NUNCHUK
	#include "rc_nunchuk.h"
#elif CONTROLLER == CLASSIC
	#include "rc_classic.h"
#endif


static const int8_t lTireTable[15][15] =  {
	{   0,   4,  12,  24,  41,  61,  86, 100, 100, 100, 100, 100, 100, 100, 100},
	{   0,   0,	  4,  12,  24,	41,	 61,  86, 100, 100, 100, 100, 100, 100, 100},
	{   0,   0,   0,   4,  12,	24,	 41,  61,  86, 100, 100, 100, 100, 100, 100},
	{   0,   0,	  0,   0,	4,	12,	 24,  41,  61,	86, 100, 100, 100, 100, 100},
	{   0,   0,	  0,   0,	0,	 4,  12,  24,  41,	61,	 86, 100, 100, 100, 100},
	{   0,	 0,   0,   0,	0,	 0,	  4,  12,  24,	41,	 61,  86, 100, 100, 100},
	{   0,   0,	  0,   0,	0,	 0,	  0,   4,  12,	24,	 41,  61,  86, 100, 100},
	{   0, 	 0,	  0,   0,	0,	 0,	  0,   0,	4,	12,	 24,  41,  61,	86, 100},
	{   0,   0,	  0,   0,	0,	 0,	 -4,  -4,   0,	 8,	 20,  37,  57,	82, 100},
	{   0,   0,	  0,   0,	0,	-8,	-12, -12,  -8,	 0,	 12,  29,  49,	73,	100},
	{   0, 	 0,	  0,   0, -12, -20,	-24, -24, -20, -12,	  0,  16,  37,	61,	 90},
	{	0,	 0,	  0, -16, -29, -37,	-41, -41, -37, -29,	-16,   0,  20,	45,	 73},
	{	0,	 0,	-20, -37, -49, -57,	-61, -61, -57, -49,	-37, -20,   0,  24,	 53},
	{	0, -24,	-45, -61, -73, -82,	-86, -86, -82, -73,	-61, -45, -24,	 0,	 29},
	{ -29, -53,	-73, -90,-100,-100,-100,-100,-100,-100,	-90, -73, -53, -29,	  0}
};

static const int8_t rTireTable[15][15] = {
	{ 100, 100,	100, 100, 100, 100,	100, 100,  86,	61,	 41,  24,  12,	 4,	  0},
	{ 100, 100,	100, 100, 100, 100,	100,  86,  61,	41,	 24,  12,	4,	 0,	  0},
	{ 100, 100,	100, 100, 100, 100,	 86,  61,  41,	24,	 12,   4,	0,	 0,	  0},
	{ 100, 100,	100, 100, 100,	86,	 61,  41,  24,	12,	  4,   0,	0,	 0,	  0},
	{ 100, 100,	100, 100,  86,	61,	 41,  24,  12,   4,	  0,   0,	0,	 0,	  0},
	{ 100, 100,	100,  86,  61,	41,	 24,  12,	4,   0,	  0,   0,	0,	 0,	  0},
	{ 100, 100,  86,  61,  41,	24,	 12,   4,	0,   0,	  0,   0,	0,	 0,	  0},
	{ 100,	86,	 61,  41,  24,	12,	  4,   0,	0,   0,	  0,   0,	0,	 0,	  0},
	{ 100,	82,	 57,  37,  20,	 8,	  0,  -4,  -4,   0,	  0,   0,	0,	 0,	  0},
	{ 100,	73,	 49,  29,  12,	 0,	 -8, -12, -12,  -8,	  0,   0,	0,	 0,	  0},
	{  90,	61,	 37,  16,	0, -12,	-20, -24, -24, -20,	-12,   0,	0,	 0,	  0},
	{  73,	45,	 20,   0, -16, -29,	-37, -41, -41, -37,	-29, -16,	0,	 0,	  0},
	{  53,	24,	  0, -20, -37, -49,	-57, -61, -61, -57,	-49, -37, -20,	 0,	  0},
	{  29,	 0,	-24, -45, -61, -73,	-82, -86, -86, -82,	-73, -61, -45, -24,	  0},
	{	0, -29,	-53, -73, -90,-100,-100,-100,-100,-100,-100, -90, -73, -53,	-29}
};

void userMain(void)
{
	union controller_data *controller;

	Motor * const lTire = getMotor(Motor0);
	Motor * const rTire = getMotor(Motor1);
	Motor * const arm = getMotor(Motor2);
	AirCylinder * const shoulder = getAirCylinder(AirCylinder0); 

	uint8_t x,y;
	int8_t lDuty,rDuty;
	bool bPushed = false, aPushed  = false, yPushed = false;
	enum MotorAction armAct = FREE, armActTmp = armAct;
	
	while ( true ) {
		controller = ToggleRCRxBuffer();
		
		if ( controller->detail.Button.A && !aPushed ) {
			armActTmp = armAct;
			armAct = CW;
			aPushed = true;
		} else if ( !controller->detail.Button.A && aPushed ) {
			armAct = armActTmp;
			aPushed = false;
		} else if ( controller->detail.Button.B && !bPushed ) {
			armActTmp = armAct;
			armAct = CCW;
			bPushed = true;
		} else if ( !controller->detail.Button.B && bPushed ) {
			armAct = armActTmp;
			bPushed = false;
		}

		if ( !aPushed && !bPushed ) {
			if ( controller->detail.Button.Y && !yPushed ) {
				armAct = ( armAct == FREE )? CW:FREE;
				yPushed = true;
			} else if ( !controller->detail.Button.Y && yPushed ) {
				yPushed = false;
			}
		}

		if ( controller->detail.Button.L ) {
			lDuty = -100;
			rDuty = 100;
			armAct = CW;
		} else if ( controller->detail.Button.R ) {
			lDuty = 100;
			rDuty = -100;
			armAct = CCW;
		} else {
			x = controller->detail.AnalogL.X;
			y = controller->detail.AnalogL.Y;
			lDuty = lTireTable[x][y];
			rDuty = rTireTable[x][y];
		}

		lTire->drive(CW,lDuty);
		rTire->drive(CW,rDuty);
		arm->drive(armAct,100);
		shoulder->drive(controller->detail.Button.X);
		

		wait(15);
	}
	
}
