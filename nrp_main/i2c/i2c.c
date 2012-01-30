#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "i2c.h"
#include "i2c_private.h"

#if MASTER_COMPILE

/** @fn void initI2CMaster(uint16_t speed) 
 * 
 * TWIをマスターとして初期化する
 *
 * @param[in] speed {uint16_t} I2Cのバススピード
 * @example initI2CMaster
 * @code
 *     initI2CMaster(100); //100kbpsでマスター動作に初期化
 * @endcode
 */
void initI2CMaster(uint8_t baud) 
{
	// ボーレートを設定し，TWI動作を許可する
		TWBR = baud;
		TWSR = 0;
		TWCR = EnableI2C;
}


/** @fn bool i2cWrite(Slave* slave)
 *
 * マスターがスレーブにwriteする
 *
 * @param[in] slave {Slave*} 送信先のスレーブインスタンスのポインタ
 * @return {bool} 成否
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
 *     i2cWrite(&slave);   // アドレスが0x01のスレーブに0xffが書かれる
 * @endcode
 */
bool i2cWrite(Slave* slave)
{
		uint8_t i, size;
	// 開始条件の生成
		TWCR = SendStartConditionBit;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentStartConditionBit ) goto ERROR;
		
	// アドレス転送，ACK確認		
		TWDR = ( slave->address << 1 ) | WRITE;
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentSlaveAddressWithWriteBitAndACK ) goto ERROR;
		
	// データバイト転送，ACK確認の一連をデータサイズ分行う
		for ( i = 0, size = slave->write.size; i < size; i++ ) {
			TWDR = slave->write.buffer[i];
			TWCR = ContinueI2C;
			while ( I2C_DOING );
			if ( I2C_STATUS != SentDataByteAndACKAtMaster ) goto ERROR;
		}
		
	// 終了条件の生成
		TWCR = SendStopConditionBit;
		
	// 正常終了
		return false;
		
	// エラーケース
ERROR:
		TWCR = SendStopConditionBit;
		return true;
}

/** @fn bool i2cRead(Slave* slave)
 *
 * マスターがスレーブにreadする
 *
 * @param[in] slave {Slave*} 受信先のスレーブインスタンスのポインタ
 * @return {bool} 成否
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
 *     i2cRead(&slave);   // アドレスが0x01のスレーブから値を1byte読む
 * @endcode
 */
bool i2cRead(Slave* slave)
{
		uint8_t i, size;
	// 開始条件の生成
		TWCR = SendStartConditionBit;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentStartConditionBit ) goto ERROR;
		
	// アドレス転送，ACK確認
		TWDR = ( slave->address << 1 ) | READ;
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentSlaveAddressWithReadBitAndACK ) goto ERROR;
		
	// データ受信，ACK発行
		for ( i = 0, size = slave->read.size - 1; i < size; i++ ) {
			TWCR = ContinueAndResponseI2C;
			while( I2C_DOING );
			if ( I2C_STATUS != ReceivedDataByteAndACKAtMaster ) goto ERROR;
			slave->read.buffer[i] = TWDR;
		}
			
	// 最終データ受信，NACK発行
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != ReceivedDataByteAndNACK ) goto ERROR;
		slave->read.buffer[size] = TWDR;
		
	// 終了条件の生成
		TWCR = SendStopConditionBit;
		
	// 正常終了
		return false;
		
	// エラーケース
ERROR:
		TWCR = SendStopConditionBit;
		return true;
}
 
/** @fn bool i2cReadWithCommand(Slvae* slave)
 *
 * マスターがスレーブにコマンドを送ってからreadする
 *
 * @param[in] slave {Slave*} 送受信先のスレーブインスタンスのポインタ
 * @return {bool} 成否
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
 *     i2cReadWithCommand(&slave,cmd);   // アドレスが0x01のスレーブにcommandを送り，つづけて値を1byte読む
 * @endcode
 */
bool i2cReadWithCommand(Slave* slave)
{
		uint8_t i, size;
	// 開始条件の生成
		TWCR = SendStartConditionBit;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentStartConditionBit ) goto ERROR;
    
	// アドレス転送
		TWDR = ( slave->address << 1 ) | READ;
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != SentSlaveAddressWithReadBitAndACK ) goto ERROR;
    
	// コマンド転送，ACK確認
		for ( i = 0, size = slave->write.size; i < size; i++ ) {
			TWDR = slave->write.buffer[i];
			TWCR = ContinueI2C;
			while ( I2C_DOING );
			if ( I2C_STATUS != SentDataByteAndACKAtMaster ) goto ERROR;
		}
    
	// データバイト受信，ACK発行の一連をデータサイズ-1分行う
		for ( i = 0, size = slave->read.size - 1; i < size; i++ ){
		    TWCR = ContinueAndResponseI2C;
		    while( I2C_DOING );
		    if ( I2C_STATUS != ReceivedDataByteAndACKAtMaster ) goto ERROR;
		    slave->read.buffer[i] = TWDR;
		}
    
	// 最終データバイト受信，NACK発行
		TWCR = ContinueI2C;
		while ( I2C_DOING );
		if ( I2C_STATUS != ReceivedDataByteAndNACK ) goto ERROR;
		slave->read.buffer[size] = TWDR;
    
	// 終了条件の生成
		TWCR = SendStopConditionBit;
    
	// 正常終了
		return false;
    
	// エラーケース
ERROR:
		TWCR = SendStopConditionBit;
		return true;
}

#endif // MASTER_COMPILE


#if SLAVE_COMPILE

/**
 * スレーブで割り込み時に使うグローバル変数の宣言
 * 割り込みで使うので扱い注意
 * @var own 自信のスレーブインスタンス
 * @var generalCall 一斉呼出を示すフラグ
 * @var i2cComFlag 通信中を示すフラグ
 */
static volatile Slave* own;
static volatile bool generalCall = false;
static volatile bool i2cComFlag = false;	

/** @fn void initI2CSlave(Slave* _own) 
 *
 * TWIをスレーブとして初期化する
 *
 * @param[in] _own {Slave*} 自分自身のスレーブインスタンスのポインタ
 * @example initI2CSlave
 * @code
 *     uint8_t recvBuf = 0, transBuf = 0;
 *     //マスター視点のwrite，readなのでスレーブだとwriteは受信，readは送信扱いになる
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
 *     initI2CSlave(&own);   // アドレスが0x01のスレーブに自身がセットされる
 *     sei();
 * @endcode
 */
void initI2CSlave(Slave* _own) 
{
	// 自アドレスを設定し，TWI，ACK，割り込み，一斉呼出しを許可する
	    own = _own;
	    TWAR = own->address << 1;
	    TWAR |= GCRE_BIT;		
	    TWCR = ACKEnableI2C | EnableI2C | InterruptEnableI2C;
}

/**
 * 一斉呼出検査関数
 * 読みだすとフラグは下がる
 * @return {bool} 有ったか無かったか
 */

bool isGeneralCalled(void)
{
	bool gc = generalCall;
	generalCall = false;
	return gc;
}


/**
 * 通信中検査関数
 * @return {bool} 通信中か否か
 */
bool isCommunicatingViaI2C(void)
{
	return i2cComFlag;
}

/**
 * スレーブ動作の時の割り込み処理
 */
ISR (TWI_vect)
{
	static uint8_t byteCount;
    switch (TWSR) {
        // 一斉呼出受信，一斉呼出と認定，ReceivedOwnAddressWithWriteBitAndACKを実行
            case ReceivedGeneralCallRecognitionAndACK:
            	generalCall = true;
            	
        // 自アドレス+W受信，TWINTを下ろす
            case ReceivedOwnAddressWithWriteBitAndACK:
                byteCount = 0;
                i2cComFlag = true;
                TWCR |= ContinueI2C;
                break;
            
        // 一斉呼出データバイト受信，ReceivedDataByteAndACKAtSlaveを実行
            case ReceivedGeneralCallRecognitionDataByteAndACK:
            
        // データバイト受信，バッファに移してTWINTを下ろす
            case ReceivedDataByteAndACKAtSlave:
                own->write.buf[byteCount++] = TWDR;
                break;
            
        // 終了条件の受信，TWINTを下ろす
            case ReceivedStopConditionBit:
                TWCR |= ContinueI2C;
                break;
            
        // 自アドレス+R受信，1バイト目のデータバイトの送信を行い，TWINTを下ろす
            case ReceivedOwnAddressWithReadBitAndACK:
                byteCount = 0;
                i2cComFlag = true;
                TWDR = own->read.buf[byteCount++];				
                TWCR |= ContinueI2C;
                break;
            
        // ACK受信，データバイトの送信を行い，TWINTを下ろす
            case SentDataByteAndACKAtSlave:
                TWDR = own->read.buf[byteCount++];
                TWCR |= ContinueI2C;
                break;
            
        // NACKの受信，TWINTを下ろす
            case SentDataByteAndNACKAtSlave:
                TWCR |= ContinueI2C;
                i2cComFlag = false;
                break;
            
        // 他，TWCRの初期化
            case default:
                TWCR = ACKEnableI2C | EnableI2C | InterruptEnableI2C;
                i2cComFlag = false;
                break;
    }
}
#endif // SLAVE_COMPILE
