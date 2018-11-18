CC = gcc
CFLAGS = -c -Wall
EXECUTABLE = reciever spammer
CONFIG_SOURCE = time_config.c
SPAM_SOURCES = $(CONFIG_SOURCE) time_spammer.c
SPAM_OBJECTS = $(SPAM_SOURCES:.c=.o)
RECV_SOURCES = $(CONFIG_SOURCE) reciever.c
RECV_OBJECTS = $(RECV_SOURCES:.c=.o)

all: $(EXECUTABLE)

reciever: $(RECV_OBJECTS)
	$(CC) $(RECV_OBJECTS) -o $@

spammer: $(SPAM_OBJECTS)
	$(CC) $(SPAM_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o reciever spammer *.exe
