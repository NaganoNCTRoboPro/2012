#include <avr/io.h>
#include <stdbool.h>
#include <i2c.h>
#include <wait.h>

void LED(unsigned char target, bool status);
void initLED(void);

int main (void)
{
	bool failed = false;
	uint8_t array[2];
	Slave slave = {
		.address = 0x01,
		.write = {
			.buffer = &array[0],
			.size = 1
		},
		.read = {
			.buffer = &array[1],
			.size = 1
		}
	};
	initI2CMaster(100);
	initUART(UART0,
		StopBitIs1Bit|NonParity,
		ReceiverEnable|TransmiterEnable,
		115200
	);
	initLED();
	DDRC |= 0x04;
	PORTC &= 0xFB;
	

	LED(0,true);
	LED(1,true);
	LED(2,true);
	wait_ms(500);
	LED(0,false);
	LED(1,false);
	LED(2,false);
	wait_ms(500);

	while ( true ) {
		failed = false;
		failed |= i2cWrite(&slave);
		LED(0,failed);
		LED(1,!failed);
		wait_ms(10);
	}
	return 0;
}

void LED(unsigned char target, bool status)
{
	if(target > 2)return;
	if(status)	PORTA &= ~_BV(target);
	else		PORTA |= _BV(target);
}

void initLED(void)
{
	DDRA |= 0x07;
	PORTA |= 0x07;
}
