 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <stdbool.h>
 #include "uart.h"
 #include "uart_private.h"
 
/**
 * UART���W�X�^�̏����� �i8bit �p���e�B�Ȃ��j
 * @param[in] no {UARTNumber} UART�̔ԍ�
 * @param[in] mode {UARTMode} UART�̃��[�h�ݒ�l�i�X�g�b�v�r�b�g�C�p���e�B�r�b�g�j
 * @param[in] act {UARTAction} UART�̓���̐ݒ�l�i���M�C��M�C���荞�݂Ȃǁj
 * @param[in] speed {uint32} UART�̒ʐM���x
 */
void initUART(enum UARTNumber no, enum UARTMode mode, enum UARTAction act,uint16_t baud)
{	
	switch ( no ){
		case UART0:
			UCSR0C = mode | OneFrameIs8Bit;
			UCSR0B = act;
			UBRR0H = (uint8_t)(baud>>8);
	 		UBRR0L = (uint8_t)baud;
			break;
		case UART1:
			UCSR1C = mode | OneFrameIs8Bit;
			UCSR1B = act;
			UBRR1H = (uint8_t)(baud>>8);
	 		UBRR1L = (uint8_t)baud;
			break;
		default:
			break;
	}
}
 
/**
 * 1Byte���M����
 * @param[in] value {uint8_t} ���M����f�[�^
 */
void uart0Put(uint8_t value)
{
	while ( UART0_TRANSMITTING );
	UDR0 = value;
}
/**
 * 1Byte���M����
 * @param[in] value {uint8_t} ���M����f�[�^
 */
void uart1Put(uint8_t value)
{
	while ( UART1_TRANSMITTING );
	UDR1 = value;
} 
 
/**
 * 1Byte��M����
 * @return {uint8_t} ��M�����f�[�^
 */
uint8_t uart0Get(void)
{
	while ( UART0_RECEIVING );
	return UDR0;
}
/**
 * 1Byte��M����
 * @return {uint8_t} ��M�����f�[�^
 */
uint8_t uart1Get(void)
{
	while ( UART1_RECEIVING );
	return UDR1;
}

/**
 * ����Byte���M����
 * @param[in] values {char*} ����f�[�^�Q
 */
void uart0Puts(char* values)
{
	while ( *values != '\0' ) {
		uart0Put(*values++);
	}
}
/**
 * ����Byte���M����
 * @param[in] values {char*} ����f�[�^�Q
 */
void uart1Puts(char* values)
{
	while ( *values != '\0' ) {
		uart1Put(*values++);
	}
}

static volatile struct RingBuffer{
	uint8_t rp,wp;
	uint8_t *buffer;
	uint8_t size;
	bool *empty
} ringBuffers[2] = {
	{
		.rp = 0,
		.wp = 0,
		.buffer = (uint8_t *)0,
		.size = 0,
		.empty = true
	},
	{
		.rp = 0,
		.wp = 0,
		.buffer = (uint8_t *)0,
		.size = 0,
		.empty = true
	}
}, *pRing;

/**
 * �����O�o�b�t�@�̎Q�Ɛ��ݒ肷��
 */
void uartSetBuffer(enum UARTNumber no, uint8_t *buffer, uint8_t size)
{
	ringBuffers[no].buffer = buffer;
	ringBuffers[no].size = size;
}

/**
 * ����Byte���M����i�񓯊��j
 * @param[in] values {char*} ����f�[�^�Q
 */
void uart0PutsBuffer(char* values)
{
	uint8_t i;
	pRing = ringBuffers[0];
	while ( *values != '\0' ) {
		if ( !pRing.empty && pRing.rp == pRing.wp ) return;
		pRing.buffer[pRing.wp++] = *values++;
		if ( pRing.wp == pRing.size ) pRing.wp = 0;
		pRing.empty = false;
	}
}
/**
 * ����Byte���M����i�񓯊��j
 * @param[in] values {char*} ����f�[�^�Q
 */
void uart1PutsBuffer(char* values)
{
	uint8_t i;
	pRing = ringBuffers[1];
	while ( *values != '\0' ) {
		if ( !pRing.empty && pRing.rp == pRing.wp ) return;
		pRing.buffer[pRing.wp++] = *values++;
		if ( pRing.wp == pRing.size ) pRing.wp = 0;
		pRing.empty = false;
	}
}

/**
 * ���M�f�[�^���W�X�^�󂫊��荞��
 */
ISR (USART0_UDRE_vect)
{
	pRing = ringBuffers[0];
	if ( !pRing.empty ) {
		UDR = pRing.buffer[pRing.rp++];
		if ( pRing.rp == pRing.size ) pRing.rp = 0;
		if ( pRing.rp == pRing.wp ) pRing.empty = true;
	}
}
/**
 * ���M�f�[�^���W�X�^�󂫊��荞��
 */
ISR (USART1_UDRE_vect)
{
	pRing = ringBuffers[1];
	if ( !pRing.empty ) {
		UDR = pRing.buffer[pRing.rp++];
		if ( pRing.rp == pRing.size ) pRing.rp = 0;
		if ( pRing.rp == pRing.wp ) pRing.empty = true;
	}
}