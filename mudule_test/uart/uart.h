#ifndef UART_H
#define UART_H

#include <avr/io.h>

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

extern void initUART(enum UARTNumber no, enum UARTMode mode, enum UARTAction act,uint32_t speed);
extern void uart0Put(uint8_t value);
extern void uart1Put(uint8_t value);
extern uint8_t uart0Get(void);
extern uint8_t uart1Get(void);
extern void uart0Puts(uint8_t* values);
extern void uart0Puts(uint8_t* values);
extern void setTransmitCompleteInterruptFunc(enum UARTNumber no, void (*f)(void));
extern void setDataRegisterEmptyFunc(enum UARTNumber no, void (*f)(void));
extern void setReceiveCompleteInterruptFunc(enum UARTNumber no,void (*f)(void));
#endif //UART_H
