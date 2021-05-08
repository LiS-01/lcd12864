all: build build/target.hex

build:
	mkdir --parents build

build/target.hex: build/target.o
	avr-objcopy -O ihex build/target.o build/target.hex

build/target.o: source/testanddelete2.c
	avr-gcc -mmcu=atmega16 -O0 source/testanddelete2.c -o build/target.o

clean:
	rm -rf build

flash: all
	avrdude -p atmega16 -P /dev/ttyACM0 -c avrispv2 -U flash:w:build/target.hex
