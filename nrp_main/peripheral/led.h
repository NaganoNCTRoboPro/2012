#ifndef LED_H
#define LED_H

#include <stdbool.h>

enum LEDNumber {
	LED0 = 0,
	LED1,
	LED2	
};

extern void initLED(void);
extern void LED(unsigned char target, bool status);


#endif
