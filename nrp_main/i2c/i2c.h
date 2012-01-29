#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <stdbool.h>

#define I2CBAUD(speed) ( ( ( F_CPU / 1000 / speed ) - 16 ) / 2 )
/**
 * @def MASTER_COMPILE
 *     �}�X�^�[������R���p�C�����邩�D
 *
 * @def SLAVE_COMPILE
 *     �X���[�u������R���p�C�����邩�D
 */
	

#define MASTER_COMPILE	(1)
#define SLAVE_COMPILE	(0)

/**
 * �X���[�u�N���X�̑���
 */
typedef struct st_i2c_slave {
	/// �X���[�u�A�h���X
	uint8_t address;
	/// write����̃o�b�t�@�Ƃ��̃T�C�Y
	struct {
		uint8_t* buffer;
		uint8_t size;
	} write;
	/// read����̃o�b�t�@�Ƃ��̃T�C�Y
	struct {
		uint8_t* buffer;
		uint8_t size;
	} read;
} Slave;

/**
 * �v���g�^�C�v�錾
 */
#if MASTER_COMPILE 
	extern void initI2CMaster(uint8_t baud);
	extern bool i2cWrite(Slave* slave);
	extern bool i2cRead(Slave* slave);
	extern bool i2cReadWithCommand(Slave* slave);
#elif SLAVE_COMPILE
	extern void initI2CSlave(Slave* _own);
	extern bool isEmergency(void);
	extern bool isCommunicatingViaI2C(void);
#else
    #warning I2C CONFIG IS NOT COMPILE
#endif // �v���g�^�C�v�錾

#endif // I2C_H
