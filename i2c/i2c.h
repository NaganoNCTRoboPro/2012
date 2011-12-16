#ifndef I2C_H
#define I2C_H

#include <avr/io.h>
#include <stdbool.h>

/** 
 * @def FREQ 
 *     マイコンの動作周波数．マスター時に使用．
 *
 * @def MASTER_COMPILE
 *     マスター動作をコンパイルするか．
 *
 * @def SLAVE_COMPILE
 *     スレーブ動作をコンパイルするか．
 */
#define FREQ		8// [MHz]

#define MASTER_COMPILE	(1)
#define SLAVE_COMPILE	(1)

/**
 * スレーブクラスの属性
 */
typedef struct st_i2c_slave {
	/// スレーブアドレス
	uint8_t address;
	/// write動作のバッファとそのサイズ
	struct {
		uint8_t* buffer;
		uint8_t size;
	} write;
	/// read動作のバッファとそのサイズ
	struct {
		uint8_t* buffer;
		uint8_t size;
	} read;
} Slave;

/**
 * プロトタイプ宣言
 */
#if MASTER_COMPILE 
	extern void initI2CMaster(uint8_t speed);
	extern bool i2cWrite(Slave* slave);
	extern bool i2cRead(Slave* slave);
	extern bool i2cReadWithCommand(Slave* slave);
#elif SLAVE_COMPILE
	extern void initI2CSlave(Slave* _own);
	extern bool isEmergency(void);
	extern bool isCommunicatingViaI2C(void);
#else
    #warning I2C CONFIG IS NOT COMPILE
#endif // プロトタイプ宣言

#endif // I2C_H
