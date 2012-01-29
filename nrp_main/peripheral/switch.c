#include "switch.h"

void initSwitch(void)
{
	DDRB &= 0xf0;
	PORTB |= 0x0f;
}

bool isSwitch(unsigned char target)
{
	unsigned char status;
	
	status = PINB ^ 0xff;
	status >>= (target-1); 

	return (bool)(status & 0x01);
}
uint8_t Switch(void)
{
	return ( PINB ^ 0xff ) & 0x0f;
}


