CC=sdcc -mgbz80
AR=ar

COMPILE=$(CC) $(CFLAGS) -Iinclude -c

all: lib

lib:
	$(COMPILE) src/gbscreen.c
	$(AR) -rc stdgb.lib *.rel

clean:
	rm -f *.asm *.lst *.rel *.sym *.lib
