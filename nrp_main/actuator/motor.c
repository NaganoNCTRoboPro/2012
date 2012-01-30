#include <avr/io.h>
#include <actuator.h>
#include "motor.h"

uint8_t speedToDuty(int8_t speed, enum MotorAction *act)
{
	uint8_t tmp1;
	uint8_t tmp2;
	uint8_t duty;

	if( speed < 0 ) {
		tmp2 = -speed;
		*act ^= 0x03;
	} else tmp2 = speed;

	duty = tmp2 << 1;
	duty += tmp1 = tmp2 >> 1;
	tmp2 = tmp1 >> 3;
	tmp1 = tmp2 >> 1;
	
	duty += tmp2 - ( ( tmp2 - tmp1 ) >> 1 );

	return duty;

}

void m0(enum MotorAction act, int8_t speed)
{
	OCR0A = speedToDuty(speed,&act);
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B0;
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B1;
}

void m1(enum MotorAction act, int8_t speed)
{
	OCR0A = speedToDuty(speed,&act);
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B0;
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B1;
}
void m2(enum MotorAction act, int8_t speed)
{
	OCR0A = speedToDuty(speed,&act);
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B0;
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B1;
}
void m3(enum MotorAction act, int8_t speed)
{
	OCR0A = speedToDuty(speed,&act);
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B0;
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B1;
}
void m4(enum MotorAction act, int8_t speed)
{
	OCR0A = speedToDuty(speed,&act);
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B0;
	PA.Port.Bit.B0 = PORT(act).Port.Bit.B1;
}

Motor motors[] = {
	{.drive = m0},
	{.drive = m1},
	{.drive = m2},
	{.drive = m3},
	{.drive = m4}
};

Motor *getMotor(enum MotorNumber no)
{
	return &motors[no];
}


