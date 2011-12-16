#include <avr/io.h>
#include <stdbool.h>
#include "mu2.h"

/**
 * �V���A�����M�֐��p�|�C���^
 * @type void(*)(void)
 */
static void (*put)(void);
/**
 * �V���A����M�p�֐��|�C���^
 * @type uint8_t(*)(void)
 */
static uint8_t (*get)(void);

/**
 * �V���A�����M�֐��p�|�C���^�Z�b�^�[�֐�
 * @param[in] f {void(*)(void)}
 */
void setMU2PutFunc(void(*f)(void))
{
	put = f;
}
/**
 * �V���A����M�֐��p�|�C���^�Z�b�^�[�֐�
 * @param[in] f {uint8_t(*)(void)}
 */
void setMU2GetFunc(uint8_t(*f)(void))
{
	get = f;
}

/**
 * 16�V�ϊ��p�e�[�u��
 * @type const char
 */
static const char ascii[16] = "0123456789ABCDEF";

/**
 * 1Byte->16�i������ϊ��֐�
 * @param[in] byte {uint8_t} �ϊ��Ώۃf�[�^
 * @param[out] string {char*} �ϊ��㕶����i�[��|�C���^
 */
void byteToString(uint8_t byte, char* string)
{
	string[0] = ascii[ ( byte >> 4 ) & 0x0F ];
	string[1] = ascii[ byte & 0x0F ];
	string[2] = '�_0';
}

/**
 * MU2�ɃR�}���h�𑗐M����֐�
 * @param[in] command {const char*} MU2�ɑ��M����R�}���h
 * @param[in] values {uint8_t*} �R�}���h�ɕt������f�[�^
 */
bool mu2Command(const char* command, uint8_t* values)
{
	uint8_t i, recv[10];
	
	put('@');
	put(command[0]);
	put(command[1]);
	for ( ; *values != '�_0'; values++ ) {
		put(*values);
	}
	put(0x0d);
	put(0x0a);
	
	for ( i = 0; i < 10; i++ ){
		recv[i] = get();
		if( i > 0 && recv[tmp-1] == 0x0d && recv[i] == 0x0a ) {
			break;
		}
	}
	if ( recv[1] == 'E' && recv[2] == 'R' ) {
		return true;
	}
	
	return false;
}
/**
 * MU2�ɃR�}���h�𑗐M���CMU2��EEPROM�ɏ������ފ֐�
 * @param[in] command {const char*} MU2�ɑ��M����R�}���h
 * @param[in] values {uint8_t*} �R�}���h�ɕt������f�[�^
 */
bool mu2CommandToEEPROM(const char *command, uint8_t* values)
{
	uint8_t i, recv[10];
	
	put('@');
	put(command[0]);
	put(command[1]);
	for ( ; *values != '�_0'; values++ ) {
		put(*values);
	}
	put('/');
	put('W');
	put(0x0d);
	put(0x0a);
	
	for ( i = 0; i < 10; i++ ){
		recv[i] = get();
		if( i > 0 && recv[tmp-1] == 0x0d && recv[i] == 0x0a ) {
			break;
		}
	}
	if ( recv[1] == 'E' && recv[2] == 'R' ) {
		return true;
	}
	
	return false;
}

/**
 * MU2�Ńf�[�^�𑗐M����֐�
 * @param[in] command {const char*} MU2�ɑ��M����R�}���h
 * @param[in] values {uint8_t*} �R�}���h�ɕt������f�[�^
 */
bool mu2TransmitData(uint8_t* values, uint8_t size)
{
	uint8_t i;
	char* strSize[3];
	byteToString(size,strSize);
	put('@');
	put('D');
	put('T');
	put(strSize[0]);
	put(strSize[1]);
	for ( i = 0; i < size; i++ ) {
		put(values[i]);
	}
	put(0x0d);
	put(0x0a);
}