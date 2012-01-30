#include "remote_control_rx.h"
#if CONTROLLER == SIXAXIS
	#include "rc_sixaxis.h"
#elif CONTROLLER == NUNCHUK
	#include "rc_nunchuk.h"
#elif CONTROLLER == CLASSIC
	#include "rc_classic.h"
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <stdbool.h>
#include <led.h>

#define PACKET_LENGTH (12)
#define CHECK_STR_LENGTH (3)

enum CtrlDataSelects {
	CtrlDataSelect0 = 0,
	CtrlDataSelect1 = 1	
};
enum RecievePhases {
	SerchHead = 0,
	AnalysisPacket
};

static volatile const char check[] = "DR=";
static uint8_t defaultCtrlData[RC_DATA_LENGTH] = RC_DEFAULT_DATA;

static union controller_data ctrlDatas[2];
static enum CtrlDataSelects ctrlDataSelecter = 0;

static volatile uint8_t *ctrlData = defaultCtrlData;

static volatile uint8_t packet[PACKET_LENGTH];
static volatile uint8_t val,*p,cnt = 0;
static volatile enum RecievePhases phase = SerchHead;

volatile uint8_t ovfCnt = 0;

/*
inline void initIntervalTimer(void)
{
	TCCR1A = 0;
	TCCR1B = 5;
	TIMSK1 = 1;
	TCNT1 = 0;
}
inline void intervalTimerReset(void)
{
	ovfCnt = 0; TCNT1 = 0;	
}
*/

void initRCRx(void){
	ctrlData = defaultCtrlData;
	phase = SerchHead;
//	initIntervalTimer();
//	intervalTimerReset();
}

union controller_data *ToggleRCRxBuffer(void)
{
	return (union controller_data *)ctrlData;
}

ISR(USART0_RX_vect)
{
	val = UDR0;

	if ( phase == AnalysisPacket ) {
		packet[cnt] = val;
		if ( cnt < CHECK_STR_LENGTH-1 ) {
			if ( val != check[cnt] ) {
				phase = SerchHead;
				LED(0,false);
			}
		} else if ( cnt==8 ) {
			p = ctrlDatas[ctrlDataSelecter].buf;
			p[0] = packet[5];
			p[1] = packet[6];
			p[2] = packet[7];
			p[3] = packet[8];
			ctrlData = p;
			ctrlDataSelecter = ( ctrlDataSelecter == CtrlDataSelect0 )? CtrlDataSelect1:CtrlDataSelect0;
			phase = SerchHead;
			wdt_reset();
			LED(0,true);
//			intervalTimerReset();
		}
		cnt++;
	} else {
		if ( val == '*' ) {
			cnt = 0;
			phase = AnalysisPacket;
		}
	}

}
/*
ISR (TIMER1_OVF_vect){
	ovfCnt++;
	if(ovfCnt>150){
		ctrlData = defaultCtrlData;
	}
}
*/
