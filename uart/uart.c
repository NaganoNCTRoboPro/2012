 #include <avr/io.h>
 #include <avr/interrupt.h>
 #include "uart.h"
 #include "uart_private.h"
 
/**
 * UARTレジスタの初期化 （8bit パリティなし）
 * @param[in] no {UARTNumber} UARTの番号
 * @param[in] mode {UARTMode} UARTのモード設定値（ストップビット，パリティビット）
 * @param[in] act {UARTAction} UARTの動作の設定値（送信，受信，割り込みなど）
 * @param[in] speed {uint32} UARTの通信速度
 */
void initUART(UARTNumber no, UARTMode mode, UARTAction act,uint32_t speed)
{
	uint32_t baud = ( ( ( FREQ * 1000000 ) >> 4 ) / speed ) - 1;
	
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
 * @param[in] values {uint8_t*} 送るデータ群
 * @param[in] size {uint8_t} 送るデータサイズ
 */
void uart1Puts(uint8_t* values, uint8_t size)
{
	for ( ; size != 0; size-- ) {
		uart0Put(*valuse++);
	}
}
/**
 * 複数Byte送信する
 * @param[in] values {uint8_t*} 送るデータ群
 * @param[in] size {uint8_t} 送るデータサイズ
 */
void uart1Puts(uint8_t* values, uint8_t size)
{
	for ( ; size != 0; size-- ) {
		uart1Put(*valuse++);
	}
}


/**
 * 送信完了割り込み動作委譲用関数ポインタ
 * @type void(*)(void)
 */
 static volatile void (*transComplete0)(void);
 /**
 * 送信完了割り込み動作委譲用関数ポインタ
 * @type void(*)(void)
 */
 static volatile void (*transComplete1)(void);

/**
 * 送信完了割り込み動作委譲用関数ポインタセッター関数
 * @param[in] no {UARTNumber} UARTの番号
 * @param[in] f {void(*)(void)} 関数ポインタ
 */
void setTransmitCompleteInterruptFunc(UARTNumber no, void (*f)(void))
{
	if ( no == UART0 )
		transComplet0 = f;
	else if ( no == UART1 )
		transComplet1 = f;
}

/**
 * 送信データレジスタ空き割り込み動作委譲用関数ポインタ
 * @type void(*)(void)
 */
static volatile void (*dataRegisterEmpty0)(void);
/**
 * 送信データレジスタ空き割り込み動作委譲用関数ポインタ
 * @type void(*)(void)
 */
static volatile void (*dataRegisterEmpty1)(void);
/**
 * 送信データレジスタ空き割り込み動作委譲用関数ポインタセッター関数
 * @param[in] no {UARTNumber} UARTの番号
 * @param[in] f {void(*)(void)} 関数ポインタ
 */
void setDataRegisterEmptyFunc(UARTNumber no, void (*f)(void))
{
	if ( no == UART0 )
		dataRegisterEmpty0 = f;
	else if ( no == UART1 )
		dataRegisterEmpty1 = f;
}

/**
 * 受信完了割り込み動作委譲用関数ポインタ
 * @type void(*)(void)
 */
 static volatile void (*receiveComplete0)(void);
 /**
 * 受信完了割り込み動作委譲用関数ポインタ
 * @type void(*)(void)
 */
 static volatile void (*receiveComplete1)(void);

/**
 * 受信完了割り込み動作委譲用関数ポインタセッター関数
 * @param[in] no {UARTNumber} UARTの番号
 * @param[in] f {void(*)(void)} 関数ポインタ
 */
 void setReceiveCompleteInterruptFunc(UARTNumber no,void (*f)(void))
 {
 	if ( no == UART0 )
		recieveComplete0 = f;
	else if ( no == UART1 )
		recieveComplete1 = f;
 }

/**
 * 送信完了割り込み
 */
ISR (USART1_TX_vect)
{
	transComplete0();
}
/**
 * 送信完了割り込み
 */
ISR (USART1_TX_vect)
{
	transComplete1();
}
 
/**
 * 送信データレジスタ空き割り込み
 */
ISR (USART0_UDRE_vect)
{
	dataRegisterEmpty0();
}
/**
 * 送信データレジスタ空き割り込み
 */
ISR (USART1_UDRE_vect)
{
	dataRegisterEmpty1();
}

/**
 * 受信完了割り込み
 */
ISR (USART0_RX_vect)
{
	recieveComplete0();
}
/**
 * 受信完了割り込み
 */
ISR (USART1_RX_vect)
{
	recieveComplete1();
}