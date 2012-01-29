#ifndef __SWITCH_H__
#define __SWITCH_H__

#include <avr/io.h>
#include "common.h"
#include <stdbool.h>

enum SWStatus {
	SW_ON = true;
	SW_OFF = false;
};

extern void initSwitch(void);
extern bool isSwitch(unsigned char target);
extern uint8_t Switch(void);

#endif
