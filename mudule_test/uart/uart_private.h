#ifndef UART_PRIVATE_H
#define UART_PRIVATE_H

/**
 * @def FREQ
 *     マイコンの動作周波数
 */
#define FREQ 8 //(MHz)

/**
 * 条件マクロ
 * @def UART0_TRANSMITTING
 *     UART0送信中
 * @def UART1_TRANSMITTING
 *     UART1送信中
 * @def UART0_RECEIVING
 *     UART0受信中
 * @def UART1_RECEIVING
 *     UART1受信中
 */
#define UART0_TRANSMITTING	( ! ( UCSR0A & _BV(UDRE0) ) ) 
#define UART1_TRANSMITTING	( ! ( UCSR1A & _BV(UDRE1) ) ) 
#define UART0_RECEIVING		( ! ( UCSR0A & _BV(RXC0) ) ) 
#define UART1_RECEIVING		( ! ( UCSR1A & _BV(RXC1) ) )

/**
 * フレーム長
 */
enum FrameSize {
	/// 5ビットフレーム
		OneFrameIs5Bit = 0x00,
	/// 6ビットフレーム
		OneFrameIs6Bit = 0x02,
	/// 7ビットフレーム
		OneFrameIs7Bit = 0x04,
	/// 8ビットフレーム
		OneFrameIs8Bit = 0x06
};

#endif
