#ifndef __REMOTE_CONTROL_RX_H__
#define __REMOTE_CONTROL_RX_H__

#define SIXAXIS (0)
#define NUNCHUK (1)
#define CLASSIC (2)

#define CONTROLLER CLASSIC

#if CONTROLLER == SIXAXIS
	#include "rc_sixaxis.h"
#elif CONTROLLER == NUNCHUK
	#include "rc_nunchuk.h"
#elif CONTROLLER == CLASSIC
	#include "rc_classic.h"
#endif

#include <avr/io.h>

#define SHOW_RC_RX_DATA		(0)

extern void initRCRx(void);
extern union controller_data *ToggleRCRxBuffer(void);

#endif
