#include <avr/io.h>
#include <stdbool.h>
#include "led.h"

void initLED(void)
{
	DDRA |= 0x07;
	PORTA |= 0x07;
}

void LED(enum LEDNumber target, bool status)
{
	if(status)	PORTA &= ~_BV(target);
	else		PORTA |= _BV(target);
}
