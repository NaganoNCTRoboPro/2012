#ifndef UART_H
#define UART_H

#include <avr/io.h>

#define UARTBAUD(speed) ( ( ( F_CPU / 16 ) / speed ) - 1 )

/**
 * UART番号
 */
enum UARTNumber {
	UART0,
	UART1
};
/**
 * UARTモード
 */
enum UARTMode {
	/// 停止1ビット
		StopBitIs1Bit = 0x00,
	/// 停止2ビット
		StopBitIs2Bit = 0x08,
	/// パリティなし
		NonParity = 0x00,
	/// 奇数パリティ
		OddParity = 0x30,
	/// 偶数パリティ
		EvenParity = 0x20,
};

/**
 * UART動作
 */
enum UARTAction {
	/// 受信完了割り込み
		ReceiveCompleteInteruptEnable = 0x80,
	/// 送信完了割り込み
		TransmitCompleteInteruptEnable = 0x40,
	/// 受信許可
		ReceiverEnable = 0x10,
	/// 送信許可
		TransmiterEnable = 0x08
};

extern void initUART(enum UARTNumber no, enum UARTMode mode, enum UARTAction act,uint16_t baud);
extern void uart0Put(uint8_t value);
extern void uart1Put(uint8_t value);
extern uint8_t uart0Get(void);
extern uint8_t uart1Get(void);
extern void uart0Puts(char* values);
extern void uart1Puts(char* values);
extern void uart0PutsBuffer(char* values);
extern void uart1PutsBuffer(char* values);
#endif //UART_H
