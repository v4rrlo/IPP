CFLAGS=-W -Wall -Werror
DEBUG_FLAG=-g

SRCS=hospital.c parse.c global.c structure.c
OBJS=$(SRCS:.c=.o)
TARGET=hospital
TARGET_DBG=hospital.dbg

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

$(TARGET).o: hospital.c structure.h global.h parse.h
	$(CC) -c $(TARGET).c

.c.o:
	cc -c $<

$(TARGET_DBG): $(TARGET_DBG).o $(OBJECTS)

debug: $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAG) -o $(TARGET_DBG) $(OBJS)

clean:
	$(RM) $(OBJS) $(TARGET) $(TARGET_DBG)
