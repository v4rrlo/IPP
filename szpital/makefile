
CCo=gcc -o
CCc=gcc -g -c
SRCS=hospital.c parse.c global.c structure.c
OBJS=$(SRCS:.c=.o)

main: $(OBJS)
	$(CCo) main $(OBJS)
hospital.o: hospital.c structure.h global.h parse.h
	$(CCc) hospital.c
.c.o:
	cc -g -c $<
clean:
	@echo usuwanie plikow *.o
	rm *.o
	@echo nawet jesli ich nie ma
