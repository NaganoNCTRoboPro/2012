#ifndef UART_PRIVATE_H
#define UART_PRIVATE_H

/**
 * @def FREQ
 *     �}�C�R���̓�����g��
 */
#define FREQ 8 //(MHz)

/**
 * �����}�N��
 * @def UART0_TRANSMITTING
 *     UART0���M��
 * @def UART1_TRANSMITTING
 *     UART1���M��
 * @def UART0_RECEIVING
 *     UART0��M��
 * @def UART1_RECEIVING
 *     UART1��M��
 */
#define UART0_TRANSMITTING	( ! ( UCSR0A & _BV(UDRE0) ) ) 
#define UART1_TRANSMITTING	( ! ( UCSR1A & _BV(UDRE1) ) ) 
#define UART0_RECEIVING		( ! ( UCSR0A & _BV(RXC0) ) ) 
#define UART1_RECEIVING		( ! ( UCSR1A & _BV(RXC1) ) )

/**
 * �t���[����
 */
enum FrameSize {
	/// 5�r�b�g�t���[��
		OneFrameIs5Bit = 0x00,
	/// 6�r�b�g�t���[��
		OneFrameIs6Bit = 0x02,
	/// 7�r�b�g�t���[��
		OneFrameIs7Bit = 0x04,
	/// 8�r�b�g�t���[��
		OneFrameIs8Bit = 0x06
};

#endif
