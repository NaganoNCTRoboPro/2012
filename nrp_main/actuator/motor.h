#ifndef MOTOR_H
#define MOTOR_H

enum MotorAction {
	FREE = 0,
	CW = 1,
	CCW = 2,
	BRAKE = 3
};

enum MotorNumber {
	Motor0 = 0,
	Motor1,
	Motor2,
	Motor3,
	Motor4
};

typedef struct st_motor {
	void (*drive)(enum MotorAction, int8_t speed);
} Motor;

extern Motor *openMotor(enum MotorNumber no);

#endif
