#ifndef UART_H
#define UART_H

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
		OddParity = 0x30
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
}

#endif //UART_H