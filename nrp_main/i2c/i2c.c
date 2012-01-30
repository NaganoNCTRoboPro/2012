#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "i2c.h"
#include "i2c_private.h"

#if MASTER_COMPILE

/** @fn void initI2CMaster(uint16_t speed) 
 * 
 * TWI���}�X�^�[�Ƃ��ď���������
 *
 * @param[in] speed {uint16_t} I2C�̃o�X�X�s�[�h
 * @example initI2CMaster
 * @code
 *     initI2CMaster(100); //100kbps�Ń}�X�^�[����ɏ�����
 * @endcode
 */
void initI2CMaster(uint8_t baud) 
{
	// �{�[���[�g��ݒ肵�CTWI�����������
		TWBR = baud;
		TWSR = 0;
		TWCR = EnableI2C;
}


/** @fn bool i2cWrite(Slave* slave)
 *
 * �}�X�^�[���X���[�u��write����
 *
 * @param[in] slave {Slave*} ���M��̃X���[�u�C���X�^���X�̃|�C���^
 * @return {bool} ����
 * @example i2cWrite
 * @code
 *     uint8_t transBuf = 0, recvBuf = 0;
 *     Slave slave = { 
 *         .address = 0x01,
 *         .write = { 
 *             .buffer = &transBuf,
 *             .size = 1
 *         },
 *         .read = {
 *             .buffer = &recvBuf,
 *             .size = 1
 *          }
 *     };
 *     transBuf = 0xff;
 *     i2cWrite(&slave);   // �A�h���X��0x01�̃X���[�u��0xff���������
 * @endcode
 */
bool i2cWrite(Slave* slave)
{
		uint8_t i, size;
	// �J�n�����̐���
		TWCR = SendStartConditionBit;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentStartConditionBit ) goto ERROR;
		
	// �A�h���X�]���CACK�m�F		
		TWDR = ( slave->address << 1 ) | WRITE;
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentSlaveAddressWithWriteBitAndACK ) goto ERROR;
		
	// �f�[�^�o�C�g�]���CACK�m�F�̈�A���f�[�^�T�C�Y���s��
		for ( i = 0, size = slave->write.size; i < size; i++ ) {
			TWDR = slave->write.buffer[i];
			TWCR = ContinueI2C;
			while ( I2C_DOING );
			if ( I2C_STATUS != SentDataByteAndACKAtMaster ) goto ERROR;
		}
		
	// �I�������̐���
		TWCR = SendStopConditionBit;
		
	// ����I��
		return false;
		
	// �G���[�P�[�X
ERROR:
		TWCR = SendStopConditionBit;
		return true;
}

/** @fn bool i2cRead(Slave* slave)
 *
 * �}�X�^�[���X���[�u��read����
 *
 * @param[in] slave {Slave*} ��M��̃X���[�u�C���X�^���X�̃|�C���^
 * @return {bool} ����
 * @example i2cRead
 * @code
 *     uint8_t transBuf = 0, recvBuf = 0;
 *     Slave slave = { 
 *         .address = 0x01,
 *         .write = { 
 *             .buffer = &transBuf,
 *             .size = 1
 *         },
 *         .read = {
 *             .buffer = &recvBuf,
 *             .size = 1
 *          }
 *     };
 *     i2cRead(&slave);   // �A�h���X��0x01�̃X���[�u����l��1byte�ǂ�
 * @endcode
 */
bool i2cRead(Slave* slave)
{
		uint8_t i, size;
	// �J�n�����̐���
		TWCR = SendStartConditionBit;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentStartConditionBit ) goto ERROR;
		
	// �A�h���X�]���CACK�m�F
		TWDR = ( slave->address << 1 ) | READ;
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentSlaveAddressWithReadBitAndACK ) goto ERROR;
		
	// �f�[�^��M�CACK���s
		for ( i = 0, size = slave->read.size - 1; i < size; i++ ) {
			TWCR = ContinueAndResponseI2C;
			while( I2C_DOING );
			if ( I2C_STATUS != ReceivedDataByteAndACKAtMaster ) goto ERROR;
			slave->read.buffer[i] = TWDR;
		}
			
	// �ŏI�f�[�^��M�CNACK���s
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != ReceivedDataByteAndNACK ) goto ERROR;
		slave->read.buffer[size] = TWDR;
		
	// �I�������̐���
		TWCR = SendStopConditionBit;
		
	// ����I��
		return false;
		
	// �G���[�P�[�X
ERROR:
		TWCR = SendStopConditionBit;
		return true;
}
 
/** @fn bool i2cReadWithCommand(Slvae* slave)
 *
 * �}�X�^�[���X���[�u�ɃR�}���h�𑗂��Ă���read����
 *
 * @param[in] slave {Slave*} ����M��̃X���[�u�C���X�^���X�̃|�C���^
 * @return {bool} ����
 * @example i2cReadWithCommand
 * @code
 *     uint8_t command = 0, recvBuf = 0;
 *     Slave slave = { 
 *         .address = 0x01,
 *         .write = { 
 *             .buffer = &command,
 *             .size = 1
 *         },
 *         .read = {
 *             .buffer = &recvBuf,
 *             .size = 1
 *          }
 *     };
 *     i2cReadWithCommand(&slave,cmd);   // �A�h���X��0x01�̃X���[�u��command�𑗂�C�Â��Ēl��1byte�ǂ�
 * @endcode
 */
bool i2cReadWithCommand(Slave* slave)
{
		uint8_t i, size;
	// �J�n�����̐���
		TWCR = SendStartConditionBit;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentStartConditionBit ) goto ERROR;
    
	// �A�h���X�]��
		TWDR = ( slave->address << 1 ) | READ;
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentSlaveAddressWithReadBitAndACK ) goto ERROR;
    
	// �R�}���h�]���CACK�m�F
		for ( i = 0, size = slave->write.size; i < size; i++ ) {
			TWDR = slave->write.buffer[i];
			TWCR = ContinueI2C;
			while ( I2C_DOING );
			if ( I2C_STATUS != SentDataByteAndACKAtMaster ) goto ERROR;
		}
    
	// �f�[�^�o�C�g��M�CACK���s�̈�A���f�[�^�T�C�Y-1���s��
		for ( i = 0, size = slave->read.size - 1; i < size; i++ ){
		    TWCR = ContinueAndResponseI2C;
		    while( I2C_DOING );
		    if ( I2C_STATUS != ReceivedDataByteAndACKAtMaster ) goto ERROR;
		    slave->read.buffer[i] = TWDR;
		}
    
	// �ŏI�f�[�^�o�C�g��M�CNACK���s
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != ReceivedDataByteAndNACK ) goto ERROR;
		slave->read.buffer[size] = TWDR;
    
	// �I�������̐���
		TWCR = SendStopConditionBit;
    
	// ����I��
		return false;
    
	// �G���[�P�[�X
ERROR:
		TWCR = SendStopConditionBit;
		return true;
}

#endif // MASTER_COMPILE


#if SLAVE_COMPILE

/**
 * �X���[�u�Ŋ��荞�ݎ��Ɏg���O���[�o���ϐ��̐錾
 * ���荞�݂Ŏg���̂ň�������
 * @var own ���M�̃X���[�u�C���X�^���X
 * @var generalCall ��Čďo�������t���O
 * @var i2cComFlag �ʐM���������t���O
 */
static volatile Slave* own;
static volatile bool generalCall = false;
static volatile bool i2cComFlag = false;	

/** @fn void initI2CSlave(Slave* _own) 
 *
 * TWI���X���[�u�Ƃ��ď���������
 *
 * @param[in] _own {Slave*} �������g�̃X���[�u�C���X�^���X�̃|�C���^
 * @example initI2CSlave
 * @code
 *     uint8_t recvBuf = 0, transBuf = 0;
 *     //�}�X�^�[���_��write�Cread�Ȃ̂ŃX���[�u����write�͎�M�Cread�͑��M�����ɂȂ�
 *     Slave own = { 
 *         .address = 0x01,
 *         .write = { 
 *             .buffer = &recvBuf,
 *             .size = 1
 *         },
 *         .read = {
 *             .buffer = &transBuf,
 *             .size = 1
 *          }
 *     };
 *     initI2CSlave(&own);   // �A�h���X��0x01�̃X���[�u�Ɏ��g���Z�b�g�����
 *     sei();
 * @endcode
 */
void initI2CSlave(Slave* _own) 
{
	// ���A�h���X��ݒ肵�CTWI�CACK�C���荞�݁C��Čďo����������
	    own = _own;
	    TWAR = own->address << 1;
	    TWAR |= GCRE_BIT;		
	    TWCR = ACKEnableI2C | EnableI2C | InterruptEnableI2C;
}

/**
 * ��Čďo�����֐�
 * �ǂ݂����ƃt���O�͉�����
 * @return {bool} �L����������������
 */

bool isGeneralCalled(void)
{
	bool gc = generalCall;
	generalCall = false;
	return gc;
}


/**
 * �ʐM�������֐�
 * @return {bool} �ʐM�����ۂ�
 */
bool isCommunicatingViaI2C(void)
{
	return i2cComFlag;
}

/**
 * �X���[�u����̎��̊��荞�ݏ���
 */
ISR (TWI_vect)
{
	static uint8_t byteCount;
    switch (TWSR) {
        // ��Čďo��M�C��Čďo�ƔF��CReceivedOwnAddressWithWriteBitAndACK�����s
            case ReceivedGeneralCallRecognitionAndACK:
            	generalCall = true;
            	
        // ���A�h���X+W��M�CTWINT�����낷
            case ReceivedOwnAddressWithWriteBitAndACK:
                byteCount = 0;
                i2cComFlag = true;
                TWCR |= ContinueI2C;
                break;
            
        // ��Čďo�f�[�^�o�C�g��M�CReceivedDataByteAndACKAtSlave�����s
            case ReceivedGeneralCallRecognitionDataByteAndACK:
            
        // �f�[�^�o�C�g��M�C�o�b�t�@�Ɉڂ���TWINT�����낷
            case ReceivedDataByteAndACKAtSlave:
                own->write.buf[byteCount++] = TWDR;
                break;
            
        // �I�������̎�M�CTWINT�����낷
            case ReceivedStopConditionBit:
                TWCR |= ContinueI2C;
                break;
            
        // ���A�h���X+R��M�C1�o�C�g�ڂ̃f�[�^�o�C�g�̑��M���s���CTWINT�����낷
            case ReceivedOwnAddressWithReadBitAndACK:
                byteCount = 0;
                i2cComFlag = true;
                TWDR = own->read.buf[byteCount++];				
                TWCR |= ContinueI2C;
                break;
            
        // ACK��M�C�f�[�^�o�C�g�̑��M���s���CTWINT�����낷
            case SentDataByteAndACKAtSlave:
                TWDR = own->read.buf[byteCount++];
                TWCR |= ContinueI2C;
                break;
            
        // NACK�̎�M�CTWINT�����낷
            case SentDataByteAndNACKAtSlave:
                TWCR |= ContinueI2C;
                i2cComFlag = false;
                break;
            
        // ���CTWCR�̏�����
            case default:
                TWCR = ACKEnableI2C | EnableI2C | InterruptEnableI2C;
                i2cComFlag = false;
                break;
    }
}
#endif // SLAVE_COMPILE
