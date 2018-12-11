CC = gcc
CFLAGS = -c -Wall
HEADRES = config.h
SENDER_SOURCES = sender.c
SENDER_OBJECTS = $(SENDER_SOURCES:.c=.o)
RECEIVER_SOURCES = receiver.c
RECEIVER_OBJECTS = $(RECEIVER_SOURCES:.c=.o)
EXECUTABLE = sender receiver

all: $(EXECUTABLE)

sender: $(SENDER_OBJECTS)
	$(CC) $(SENDER_OBJECTS) -o $@

receiver: $(RECEIVER_OBJECTS)
	$(CC) $(RECEIVER_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o

clean_all:
	rm -f *.o *.exe $(EXECUTABLE)
