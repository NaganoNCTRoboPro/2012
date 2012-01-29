 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include <stdbool.h>
 #include "uart.h"
 #include "uart_private.h"
 
/**
 * UARTレジスタの初期化 （8bit パリティなし）
 * @param[in] no {UARTNumber} UARTの番号
 * @param[in] mode {UARTMode} UARTのモード設定値（ストップビット，パリティビット）
 * @param[in] act {UARTAction} UARTの動作の設定値（送信，受信，割り込みなど）
 * @param[in] speed {uint32} UARTの通信速度
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
 * 1Byte送信する
 * @param[in] value {uint8_t} 送信するデータ
 */
void uart0Put(uint8_t value)
{
	while ( UART0_TRANSMITTING );
	UDR0 = value;
}
/**
 * 1Byte送信する
 * @param[in] value {uint8_t} 送信するデータ
 */
void uart1Put(uint8_t value)
{
	while ( UART1_TRANSMITTING );
	UDR1 = value;
} 
 
/**
 * 1Byte受信する
 * @return {uint8_t} 受信したデータ
 */
uint8_t uart0Get(void)
{
	while ( UART0_RECEIVING );
	return UDR0;
}
/**
 * 1Byte受信する
 * @return {uint8_t} 受信したデータ
 */
uint8_t uart1Get(void)
{
	while ( UART1_RECEIVING );
	return UDR1;
}

/**
 * 複数Byte送信する
 * @param[in] values {char*} 送るデータ群
 */
void uart0Puts(char* values)
{
	while ( *values != '\0' ) {
		uart0Put(*values++);
	}
}
/**
 * 複数Byte送信する
 * @param[in] values {char*} 送るデータ群
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
 * リングバッファの参照先を設定する
 */
void uartSetBuffer(enum UARTNumber no, uint8_t *buffer, uint8_t size)
{
	ringBuffers[no].buffer = buffer;
	ringBuffers[no].size = size;
}

/**
 * 複数Byte送信する（非同期）
 * @param[in] values {char*} 送るデータ群
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
 * 複数Byte送信する（非同期）
 * @param[in] values {char*} 送るデータ群
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
 * 送信データレジスタ空き割り込み
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
 * 送信データレジスタ空き割り込み
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