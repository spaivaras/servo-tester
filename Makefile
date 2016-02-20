# Name: Makefile
# Author: Aivaras
# Licence: @Licence

DEVICE     = atmega8
CLOCK      = 8000000
PROGRAMMER = usbasp
OBJECTS    = servo.o
FUSES      = -U lfuse:w:0xe4:m -U hfuse:w:0xdf:m 

AVRDUDE = avrdude -c $(PROGRAMMER) -p $(DEVICE)
COMPILE = avr-gcc -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	SERVOS.hex

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:SERVOS.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

clean:
	rm -f SERVOS.hex SERVOS.elf $(OBJECTS)

# file targets:
SERVOS.elf: $(OBJECTS)
	$(COMPILE) -o SERVOS.elf $(OBJECTS)

SERVOS.hex: SERVOS.elf
	rm -f SERVOS.hex
	avr-objcopy -j .text -j .data -O ihex SERVOS.elf SERVOS.hex
	avr-size --format=avr --mcu=$(DEVICE) SERVOS.elf
	
# Targets for code debugging and analysis:
disasm:	SERVOS.elf
	avr-objdump -d SERVOS.elf

cpp:
	$(COMPILE) -E SERVOS.c
