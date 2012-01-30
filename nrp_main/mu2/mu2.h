#ifndef MU2_H
#define MU2_H

#include <avr/io.h>
#include <stdbool.h>

extern void setMU2PutFunc(void(*f)(uint8_t));
extern void setMU2GetFunc(uint8_t(*f)(void));
extern void byteToString(uint8_t byte, char *string);
extern bool mu2Command(const char *command, char* values);
extern bool mu2CommandToEEPROM(const char *command, uint8_t* values);
extern bool mu2TransmitData(uint8_t* values, uint8_t size);

#endif //MU2_H
