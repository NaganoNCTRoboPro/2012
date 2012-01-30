###############################################################################
# Makefile for the project nrp
###############################################################################

## General Flags
PROJECT = nrp
MCU = atmega1284p
TARGET = nrp.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99   -DF_CPU=20000000UL -O0 -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=nrp.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Users\iwate\Desktop\github\2012\nrp_main\." -I"C:\Users\iwate\Desktop\github\2012\nrp_main\i2c" -I"C:\Users\iwate\Desktop\github\2012\nrp_main\mu2" -I"C:\Users\iwate\Desktop\github\2012\nrp_main\peripheral" -I"C:\Users\iwate\Desktop\github\2012\nrp_main\remote_control" -I"C:\Users\iwate\Desktop\github\2012\nrp_main\uart" -I"C:\Users\iwate\Desktop\github\2012\nrp_main\wait" 

## Objects that must be built in order to link
OBJECTS = main.o user_main.o i2c.o mu2.o led.o switch.o remote_control_rx.o uart.o wait.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) nrp.hex nrp.eep nrp.lss size

## Compile
main.o: ../main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

user_main.o: ../user_main.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

i2c.o: ../i2c/i2c.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

mu2.o: ../mu2/mu2.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

led.o: ../peripheral/led.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

switch.o: ../peripheral/switch.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

remote_control_rx.o: ../remote_control/remote_control_rx.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: ../uart/uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

wait.o: ../wait/wait.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) nrp.elf dep/* nrp.hex nrp.eep nrp.lss nrp.map


## Other dependencies
-include $(shell mkdir dep 2>NUL) $(wildcard dep/*)

