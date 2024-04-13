BAUD=57600 # Nano (old bootloader)
AVR=atmega328p
FREQ=16000000
DEV=/dev/tty.usbserial-1110
PROGRAMMER=arduino

FLAGS=-DF_CPU=$(FREQ) -mmcu=$(AVR) -Wall -Werror -Wextra -Os
CFLAGS=$(FLAGS) -Wno-multichar -Wno-overflow -Wno-unknown-pragmas -Wno-type-limits -Wno-sign-compare -std=gnu99
CPPFLAGS=$(FLAGS) -std=c++11

OBJECTS=$(patsubst %.cpp,%.o,$(wildcard *.cpp)) $(patsubst %.c,%.o,$(wildcard *.c))

.PHONY: flash clean

all: main.hex

%.o: %.cpp
	avr-gcc $(CPPFLAGS) -c $< -o $@

%.o: %.c
	avr-gcc $(CFLAGS) -c $< -o $@

main.elf: $(OBJECTS)
	avr-gcc $(CFLAGS) -o $@ $^

main.hex: main.elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

flash: main.hex
	avrdude -p$(AVR) -c$(PROGRAMMER) -P$(DEV) -b$(BAUD) -v -U flash:w:$<

clean:
	rm -f main.hex main.elf $(OBJECTS)
