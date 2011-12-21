#ifndef UART_H
#define UART_H

#include <avr/io.h>

/**
 * UART�ԍ�
 */
enum UARTNumber {
	UART0,
	UART1
};
/**
 * UART���[�h
 */
enum UARTMode {
	/// ��~1�r�b�g
		StopBitIs1Bit = 0x00,
	/// ��~2�r�b�g
		StopBitIs2Bit = 0x08,
	/// �p���e�B�Ȃ�
		NonParity = 0x00,
	/// ��p���e�B
		OddParity = 0x30,
	/// �����p���e�B
		EvenParity = 0x20,
};

/**
 * UART����
 */
enum UARTAction {
	/// ��M�������荞��
		ReceiveCompleteInteruptEnable = 0x80,
	/// ���M�������荞��
		TransmitCompleteInteruptEnable = 0x40,
	/// ��M����
		ReceiverEnable = 0x10,
	/// ���M����
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
