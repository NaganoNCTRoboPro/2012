#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <wait.h>
#include <i2c.h>
#include <uart.h>
#include <mu2.h>
#include <remote_control_rx.h>
#include <led.h>
#include <switch.h>
#include <user_main.h>

#include <motor.h>

char * const EI[] = { "01", "03", "07", "0F", "1F" };
char * const DI[] = { "80", "C0", "E0", "F0", "F8" };
char * const GI[] = { "21", "9A", "44", "80", "12" };
char * const CH[] = { "08", "10", "19", "23", "2E" };

int main (void)
{
	uint8_t no = 0;
//	wait(10);
	setMU2PutFunc(uart0Put);
	setMU2GetFunc(uart0Get);
	initUART(
		UART0,
		StopBitIs1Bit|NonParity,
		ReceiverEnable|TransmiterEnable|ReceiveCompleteInteruptEnable,
		UARTBAUD(19200)
	);
	initLED();
	initSwitch();
	
	initRCRx();
	
	no = Switch() & 0x03;
	
	mu2Command("EI",EI[no]);
	mu2Command("DI",DI[no]);
	mu2Command("GI",GI[no]);
	mu2Command("CH",CH[no]);

	sei();
	userMain();
	
	return 0;
}

