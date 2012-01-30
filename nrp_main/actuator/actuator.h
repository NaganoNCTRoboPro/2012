#ifndef ACTUATOR_H
#define ACTUATOR_H

#include <avr/io.h>

struct st_port {
    union {
        unsigned char Byte;
        struct {
            unsigned char B0:1;
            unsigned char B1:1;
            unsigned char B2:1;
            unsigned char B3:1;
            unsigned char B4:1;
            unsigned char B5:1;
            unsigned char B6:1;
            unsigned char B7:1;
        } Bit;
    } Port;
};

#define PA (*(volatile struct st_port*)0x02)
#define PB (*(volatile struct st_port*)0x05)
#define PC (*(volatile struct st_port*)0x08)
#define PD (*(volatile struct st_port*)0x0B)
#define PORT(sig) (*(volatile struct st_port *)&sig)

#endif
