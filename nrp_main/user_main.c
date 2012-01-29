#include <remote_control.h>
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


void userMain(void)
{
	static union controller_data *controller;
	static bool failed = false;
	
	while ( true ) {
		wait(15);
	}
	
}