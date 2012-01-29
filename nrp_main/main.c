#include <avr/io.h>
#include <stdbool.h>
#include <wait.h>
#include <i2c.h>
#include <uart.h>
#include <mu2.h>
#include <remote_control.h>
#include <led.h>
#include <switch.h>
#include <user_main.h>

const char * const EI[] = { "01", "03", "07", "0F", "1F" }
const char * const DI[] = { "80", "C0", "E0", "F0", "F8" }
const char * const GI[] = { "21", "9A", "44", "80", "12" }
const char * const CH[] = { "08", "10", "19", "23", "2E" }

int main (void)
{
	uint8_t no;
	wait(10);
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
	
	mu2_command("EI",EI[no]);
	mu2_command("DI",DI[no]);
	mu2_command("GI",GI[no]);
	mu2_command("CH",CH[no]);

	userMain();
	
	return 0;
}

