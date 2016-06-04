CC=sdcc --opt-code-size -mgbz80
AR=ar

COMPILE=$(CC) $(CFLAGS) -c

all: lib

lib:
	$(COMPILE) stdgb.c
	$(AR) -rc stdgb.lib *.rel

clean:
	rm -f *.asm *.lst *.rel *.sym *.lib
