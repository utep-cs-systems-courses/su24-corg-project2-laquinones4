# makfile configuration
COMMON_OBJECTS  = p1_interrupt_handler.o led.o switches.o buzzer.o main.o
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -I../h
LDFLAGS		= -L/opt/ti/msp430_gcc/include

#switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-as

all: playbuzzer.elf

#additional rules for files
playbuzzer.elf: ${COMMON_OBJECTS} ../lib/libTimer.a
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^  

load: playbuzzer.elf
	msp430loader.sh playbuzzer.elf

clean:
	rm -f *.o *.elf *~

#main.o: buzzer.h

#buzzer.o: buzzer.h

#p1_interrupt_handler.o: switches.h

#led.o: led.h switches.h

#switches.o: led.h switches.h

#wdInterruptHandler.o:  buzzer.h
